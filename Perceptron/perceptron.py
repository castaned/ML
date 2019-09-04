"""==============================================================================
                   Perceptron Class to train
=============================================================================="""
import numpy as np


class Perceptron(object):

    def __init__(self, n_features, threshold=0.001, learning_rate=0.01):
        self.threshold = threshold
        self.learning_rate = learning_rate
        self.weights = np.random.uniform(0.0, 1.0, n_features + 1)
        self.cum_error = 0.0
        self.loss = []

    def predict(self, features):
        sum_val = self.weights[0] + np.dot(features, self.weights[1:])
        return np.sign(sum_val)

    def train(self, x_dataset, y_label):
        while True:
            self.cum_error = 0.0
            inst_loss = 0.0
            for instance, target in zip(x_dataset, y_label):
                predicted = self.predict(instance)
                error = target - predicted
                self.weights[1:] += self.learning_rate * error * instance
                self.weights[0] += self.learning_rate * error
                if error != 0:
                    self.cum_error += 1 / len(y_label)      # max(cum_error) = len(y_label) i.e. all wrong
                    inst_loss += error*error
            self.loss.append(inst_loss/len(x_dataset))      # Average loss over all examples
            if self.cum_error < self.threshold:
                break
        return self.weights, self.loss
