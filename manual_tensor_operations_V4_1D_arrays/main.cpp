#include <vector>
#include <iostream>
#include "weights.h"
#include "operations.h"
#include "input.h"

int main() {
    
    int input_length = first_input_length;
    int num_of_channels = first_num_of_channels;

    // load input vector into flat array
    float* input = new float[input_length*num_of_channels]();
    for (int i = 0; i < input_length; ++i) {
        for (int j = 0; j < num_of_channels; ++j) {
            input[i * num_of_channels + j] = test_data[i][j];
        }
    }

    // perform separable convolution 1ST LAYER
    float* depthwise_result_01 = depthwise_conv1d(input, depthwise_weights_01, stride_01, input_length, num_of_channels);
    float* pointwise_result_01 = pointwise_conv1d(depthwise_result_01, pointwise_weights_01, input_length, num_of_channels);
    batch_normalization(pointwise_result_01, gamma_coeff_01, beta_01, moving_mean_01, moving_variance_01, input_length, num_of_channels, epsilon);
    relu(pointwise_result_01, input_length, num_of_channels);

    // perform separable convolution 2ND LAYER
    float* depthwise_result_02 = depthwise_conv1d(pointwise_result_01, depthwise_weights_02, stride_02, input_length, num_of_channels);
    float* pointwise_result_02 = pointwise_conv1d(depthwise_result_02, pointwise_weights_02, input_length, num_of_channels);
    batch_normalization(pointwise_result_02, gamma_coeff_02, beta_02, moving_mean_02, moving_variance_02, input_length, num_of_channels, epsilon);
    relu(pointwise_result_02, input_length, num_of_channels);

    // perform global pooling before dense layer
    float* global_pooling_result = global_pooling1D(pointwise_result_02, input_length, num_of_channels);

    // performs final dense layer
    float* fully_connected_result = fully_connected(global_pooling_result, dense_weights, input_length, num_of_channels);

    std::cout << input_length << " + " << num_of_channels << "\n";
    for(int i = 0; i < 1; ++i){
        std::cout << fully_connected_result[i] << ", ";
    }

    delete[] fully_connected_result;

}
