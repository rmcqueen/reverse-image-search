set(core_SRCS
        main.cpp
        Surf.cpp
        utils.cpp
        Vocabulary.cpp
        Histogram.cpp
        SVM.cpp)

add_library(core ${core_SRCS})
