"""==============================================================================
                   Perceptron Class to train
=============================================================================="""
import numpy as np


class Perceptron(object):

    def __init__(self, n_features, threshold=0.001, learning_rate=0.01):
        self.threshold = threshold
        self.learning_rate = learning_rate
        self.weights = np.random.uniform(0.0, 1.0, n_features + 1)
        self.cumm_error = 0.0

    def predict(self, features):
        sum_val = self.weights[0] + np.dot(features, self.weights[1:])
        return np.sign(sum_val)

    def train(self, x_dataset, y_label):
        while True:
            self.cumm_error = 0.0
            for instance, target in zip(x_dataset, y_label):
                predicted = self.predict(instance)
                error = target - predicted
                self.weights[1:] += self.learning_rate * error * instance
                self.weights[0] += self.learning_rate * error
                if error != 0:
                    self.cumm_error += 1 / len(y_label)
            if self.cumm_error < self.threshold:
                break
        return self.weights
