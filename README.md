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
* Move the generated executable to the root directory 
```mv reverse-image-search ..```

## Usage
* Run the following command from the root project directory: 

    ```reverse-image-search path/to/query_image.png data/images/```

    Note: this process can take an extremely long time as it must:
    * Extract all of the SIFT features for every image within the data set
    * Build a vocabulary model for the Bag of Visual  Words
    * Train a SVM model

    It was found to take approximately eight hours to complete an initial start to finish image query. However, the models only need to be built once.

## Future Work
* Replace the SVM with a convolutional NN, or some other high-performing classifier technique
* Save the extracted SIFT features to disk to reduce the amount of time it takes to query an image
* Look into a better similarity scoring technique. Cross-correlation between the bag of visual words histograms may not be the best approach


## Contributing
* If you find an issue with any of the outlined components, please create an issue with the component name in the title and a description indicating what the problem is, when it occurred, and what steps need to be taken to reproduce the issue.
* For pull requests, unit tests are mandatory to ensure the changes being incorporated function proeprly. Additionally, code is expected to be documented (see: https://google.github.io/styleguide/cppguide.html)

## License
[MIT License](https://github.com/rmcqueen/reverse-image-search/blob/master/LICENSE)