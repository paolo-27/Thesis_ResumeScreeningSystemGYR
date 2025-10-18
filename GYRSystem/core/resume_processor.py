# core/resume_processor.py

import joblib
import numpy as np
import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.feature_extraction.text import TfidfVectorizer

# --- Configuration & Model Loading ---
MODEL_DIR = 'models/'
XGB_MODEL_PATH = MODEL_DIR + 'xgb_classifier.pkl'
TFIDF_MODEL_PATH = MODEL_DIR + 'tfidf_vectorizer.pkl'

# Placeholders for loaded models
XGB_MODEL = None
TFIDF_VECTORIZER = None
EMBEDDING_DIM = 384  # Common size for SBERT models

try:
    XGB_MODEL = joblib.load(XGB_MODEL_PATH)
    TFIDF_VECTORIZER = joblib.load(TFIDF_MODEL_PATH)
    print("Models (XGBoost, TFIDF) loaded successfully.")
except FileNotFoundError:
    print("WARNING: Models not found. Run train_model.py first.")

# --- Semantic Embedding Placeholder ---
# NOTE: In a real system, you would load an SBERT model here (e.g., from sentence-transformers)
# The current implementation returns random features as a placeholder.
def get_semantic_embedding(text):
    """
    Placeholder for SBERT or other semantic embedding logic.
    A real SBERT model would return a fixed-size vector (e.g., 384 or 768 dimensions).
    """
    # Using a hash of the text to ensure the "embedding" is deterministic for testing
    np.random.seed(len(text) % 100)
    return np.random.rand(EMBEDDING_DIM) 

# --- Feature Engineering ---
def combine_features(job_desc, resume_text):
    """
    Extracts hybrid features (TF-IDF and Semantic) and returns a single feature vector.
    """
    if TFIDF_VECTORIZER is None:
        return None

    # 1. Lexical Feature (TF-IDF) - Compute similarity using pre-trained vectorizer
    # We transform the texts and then compute cosine similarity as a feature
    documents = [job_desc, resume_text]
    tfidf_vectors = TFIDF_VECTORIZER.transform(documents)
    
    # Calculate Cosine Similarity for TF-IDF as a feature
    tfidf_similarity = cosine_similarity(tfidf_vectors[0], tfidf_vectors[1])[0][0]
    
    # 2. Semantic Features (SBERT Embeddings) - Compute similarity
    jd_emb = get_semantic_embedding(job_desc)
    res_emb = get_semantic_embedding(resume_text)
    
    # Calculate Cosine Similarity for Semantic Embeddings as a feature
    # Reshape is needed for cosine_similarity function
    semantic_similarity = cosine_similarity(jd_emb.reshape(1, -1), res_emb.reshape(1, -1))[0][0]
    
    # 3. Combine Features: This feature vector is what XGBoost is trained on.
    # In a real system, you might include the raw embeddings as features too, 
    # but here we use the similarities as the core features for simplicity.
    feature_vector = [tfidf_similarity, semantic_similarity]

    return feature_vector

# --- Main Ranking Function ---
def rank_candidates(job_description, resumes_data, top_percent=30, middle_percent=50):
    """
    Uses the trained XGBoost model to classify candidates and apply GYR ranking.
    """
    if XGB_MODEL is None or not resumes_data:
        return pd.DataFrame()

    features_list = []
    
    # 1. Feature Extraction Loop
    for name, text in resumes_data.items():
        features = combine_features(job_description, text)
        if features:
            features_list.append(features)

    if not features_list:
        return pd.DataFrame()

    # Convert features to a format XGBoost can use
    X_test = np.array(features_list)

    # 2. Classification (XGBoost) - Get the probability of being 'qualified' (class 1)
    # This score is the basis for the GYR ranking.
    match_scores = XGB_MODEL.predict_proba(X_test)[:, 1]

    # Create initial results DataFrame
    results_df = pd.DataFrame({
        "Resume Name": list(resumes_data.keys()),
        "Raw Score": match_scores,
    }).sort_values(by="Raw Score", ascending=False).reset_index(drop=True)

    # 3. GYR Ranking Implementation (Objective 1.d)
    total_candidates = len(results_df)
    
    # Define thresholds based on the specified percentages (Top 30%, Middle 50%, Bottom 20%)
    green_count = int(np.ceil(total_candidates * top_percent / 100))
    yellow_count = int(np.ceil(total_candidates * middle_percent / 100))
    # Red count is the rest
    
    # Assign GYR categories
    results_df['GYR Rank'] = 'Red (Less Suitable)'
    
    # Assign Green Rank (Top Candidates)
    results_df.loc[:green_count - 1, 'GYR Rank'] = 'Green (Highly Qualified)'
    
    # Assign Yellow Rank (Middle Candidates)
    results_df.loc[green_count : green_count + yellow_count - 1, 'GYR Rank'] = 'Yellow (Moderately Qualified)'
    
    # Add final rank and format score
    results_df['Rank'] = results_df.index + 1
    results_df['Match Score'] = (results_df['Raw Score'] * 100).round(1).astype(str) + '%'
    
    return results_df[['Rank', 'Resume Name', 'Match Score', 'GYR Rank', 'Raw Score']]