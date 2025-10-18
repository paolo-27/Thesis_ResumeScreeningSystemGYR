#THIS IS INSECT RECOGNITION. IT USED IMAGES OF BUTTERFLY, DRAGONFLY, GRASSHOPPER, LADYBIRD AND MOSQUITO.
#FER PAOLO M. ESPINORIO BSCOS 302 - C

import tensorflow as tf
from tensorflow.keras import models, layers
import matplotlib.pyplot as plt
from IPython.display import HTML
from tensorflow.keras.preprocessing.image import ImageDataGenerator
import numpy as np
import warnings

print("Num GPUs Available:", len(tf.config.experimental.list_physical_devices('GPU')))

IMAGE_H = 256
IMAGE_W = 256
BATCH_SIZE = 32
CHANNELS = 3

train_datagen = ImageDataGenerator(
    rescale=1./255,
    rotation_range=40,
    width_shift_range=0.2,
    height_shift_range=0.2,
    shear_range=0.2,
    zoom_range=0.2,
)

train_generator = train_datagen.flow_from_directory(
    'insect_dataset/train',  
    target_size=(IMAGE_H, IMAGE_W),
    class_mode='sparse',
    
)

validation_datagen = ImageDataGenerator(
    rescale=1./255,
)

validation_generator = validation_datagen.flow_from_directory(
    'insect_dataset/val',  
    target_size=(IMAGE_H, IMAGE_W),
    class_mode='sparse',
)

dataset = tf.keras.preprocessing.image_dataset_from_directory(
    'insect_dataset/train',
    shuffle=True,
    image_size=(IMAGE_H, IMAGE_W),
)

class_names = dataset.class_names
class_names = ['BUTTERFLY', 'DRAGONFLY', 'GRASSHOPPER', 'LADYBIRD', 'MOSQUITO']

test_datagen = ImageDataGenerator(
    rescale=1./255,
)

test_generator = test_datagen.flow_from_directory(
    'insect_dataset/test',
    target_size=(IMAGE_H, IMAGE_W),
    class_mode='sparse',
)

input_shape = (IMAGE_H, IMAGE_W, CHANNELS)
n_classes = 5

model = tf.keras.models.Sequential([
    
    tf.keras.layers.Conv2D(16, (3, 3), activation='relu', input_shape=input_shape),
    tf.keras.layers.MaxPooling2D((2, 2)),
    
    tf.keras.layers.Conv2D(32, (3, 3), activation='relu'),
    tf.keras.layers.MaxPooling2D((2, 2)),
    
    tf.keras.layers.Conv2D(64, (3, 3), activation='relu'),
    tf.keras.layers.MaxPooling2D((2, 2)),
    
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(256, activation='relu'),
    tf.keras.layers.Dense(n_classes, activation='softmax')
])

model.summary()
Model: "sequential"

model.compile(optimizer='adam',
    loss = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=False),
    metrics=['accuracy'])

299/32 

model.fit(
    train_generator,
    validation_data=validation_generator,
    verbose=1,
    shuffle=True,
    epochs=10,
)

scores = model.evaluate(test_generator)


for image_batch, labels_batch in test_generator:
    
    first_image = image_batch[0]
    first_label = int(labels_batch[0])
    print("First image to predict:")
    plt.imshow(first_image)
    plt.axis('off')
    plt.show()
    print("actual label:", class_names[first_label])
    
    batch_predictions = model.predict(image_batch)
    print("predicted label:", class_names[np.argmax(batch_predictions[0])])
    
    break

def predict(model, img):
    img_array = tf.keras.preprocessing.image.img_to_array(img[1])
    img_array = tf.expand_dims(img_array, 0)
    
    predictions = model.predict(img_array)
    
    predicted_class = class_names[np.argmax(predictions[0])]
    confidence = np.max(predictions[0])
    return predicted_class, confidence

plt.figure(figsize=(15,15))
for images, labels in test_generator:
    for i in range(9):
        ax = plt.subplot(3,3,i+1)
        plt.imshow(images[i])
        
        predicted_class, confidence = predict(model, images[i])
        actual_class = class_names[int(labels[i])]
        
        plt.title(f"actual: {actual_class}), \n Predicted: {predicted_class}, \n Confidence: {confidence}")
        plt.axis("off")
    break

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

with open('insect_recognition_model.tflite', 'wb') as f:
    f.write(tflite_model)