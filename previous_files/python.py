import tensorflow as tf
from tensorflow.keras import models, layers
import matplotlib.pyplot as plt
from IPython.display import HTML
from tensorflow.keras.preprocessing.image import ImageDataGenerator
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
    validation_split=0.2 
)

train_generator = train_datagen.flow_from_directory(
    'banana_disease_dataset/train',  
    target_size=(IMAGE_H, IMAGE_W),
    class_mode='sparse',
    
)


validation_generator = train_datagen.flow_from_directory(
    'banana_disease_dataset/val',  
    target_size=(IMAGE_H, IMAGE_W),
    class_mode='sparse',
    batch_size=BATCH_SIZE,
    subset='validation',
    shuffle=True
)

dataset = tf.keras.preprocessing.image_dataset_from_directory(
    'banana_disease_dataset/train',
    shuffle=True,
    image_size=(IMAGE_H, IMAGE_W),
)

class_names = ['BACTERIAL SOFT ROT',
               'BLACK SIGATOKA',
               'BUNCHY TOP VIRUS',
               'CORDANA LEAF SPOT',
               'PANAMA DISEASE',
               'PSEUDOSTEM WEEVIL']
