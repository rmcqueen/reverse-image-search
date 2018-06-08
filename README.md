# reverse-image-search
Utilizing SURF, Bag of Visual Words, and a Support Vector Machine in order to identify and return similar images within the Caltech-256 data set based on a query image. Aims to compromise accuracy for real-time search speeds.

## Overview
![High-level system overview](https://github.com/rmcqueen/reverse-image-search/blob/master/docs/README/architecture.png)

## Prerequisites
* OpenCV 3.1
* Boost (https://www.boost.org)
* Caltech-256 data set (http://www.vision.caltech.edu/Image_Datasets/Caltech256/)

## Installation
* Clone the project

    ```git clone https://github.com/rmcqueen/reverse-image-search```
* Navigate to the root project directory and input the following command:

    ```mkdir data && cd data && mkdir images/ histograms```
    
    This will create the necessary folder structures to hold the data used for training the model
* Move the Caltech-256 data to `data/images/`

    ```mv path/to/caltech-256/* images/```

## Build
* Make a `build` directory and navigate to this directory
* Run ```cmake ..```
* Move the generated executable to the root directory ```mv reverse-image-search ..``

## Usage
* Run the following command from the root project directory: 

    ```reverse-image-search path/to/query_image.png data/images/```


## License
[MIT License](https://github.com/rmcqueen/reverse-image-search/blob/master/LICENSE)