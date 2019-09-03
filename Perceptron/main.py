import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from perceptron import Perceptron

"""==============================================================================
                       Generate sample data set
=============================================================================="""


def set_label(x, y, p):
    return np.sign(p[1] * x + p[0] - y)


# Sample n_dots random points in 2D.

def gen_sample(n_dots, l_lim, h_lim, p):
    data = pd.DataFrame(columns=['x1', 'x2', 'label'])
    data.x1 = np.random.uniform(l_lim, h_lim, n_dots)
    data.x2 = np.random.uniform(l_lim, h_lim, n_dots)
    data.label = set_label(data.loc[:, 'x1'], data.loc[:, 'x2'], p)
    data.to_csv('data.csv')
    return data


def plot_line(l_lim, h_lim, p):
    x = np.linspace(l_lim, h_lim, 2)
    y = p[1] * x + p[0]
    plt.plot(x, y, color='orange', label='Objective Line')
    return None


def count_vals(column):
    p = 0
    m = 0
    for i in range(len(column)):
        if column[i] > 0:
            p += 1
        else:
            m += 1
    return p, m


def test_plot(data, weights):
    x = np.linspace(0., 1., 2)
    y = -(weights[0] + weights[1] * x) / weights[2]
    plt.plot(x, y, color='red', label='Model')
    plt.scatter(data.x1, data.x2, c=data.predicted, cmap='cividis')
    plt.xlim(0., 1.)
    plt.ylim(0., 1.)
    plt.legend(loc='best')
    plt.title("Test data set")
    plt.savefig('02_test_data.png')
    plt.clf()
    return None


"""==============================================================================
                               Main
=============================================================================="""


def main():
    # Classify with arbitrary condition. Parameter list were the position is the
    # corresponding power of x and the parameter value is the value in list.
    n_dots = 100
    params = [-2.0, 5.0]
    train_data = gen_sample(n_dots, 0.0, 1.0, params)

    plt.figure(figsize=(5, 5), dpi=150)
    plot_line(0.0, 1.0, params)
    plt.scatter(train_data.x1, train_data.x2, c=train_data.label, cmap='cividis')
    plt.xlim(0., 1.)
    plt.ylim(0., 1.)
    plt.legend(loc='best')
    plt.title("Training data set")
    plt.savefig('01_training_data.png')
    plt.clf()

    # Slice data frame into features and labels. ===============================
    x = train_data[['x1', 'x2']].values
    y = train_data.loc[:, 'label']
    model = Perceptron(x.shape[1])  # Initialize model with two features.
    w = model.train(x, y)  # Train model.

    # Validate model generating a new data set. ================================
    plot_line(0.0, 1.0, params)
    test_data = gen_sample(n_dots, 0.0, 1.0, params)
    test_data['predicted'] = np.nan  # Add new column for predictions
    test_data.predicted = model.predict(test_data[['x1', 'x2']].values)

    pl, ml = count_vals(test_data.label.values)  # Compare predictions to targets
    pp, mp = count_vals(test_data.predicted.values)
    print("Bias: {:4.5}, W1: {:3.5}, W2: {:3.5}".format(w[0], w[1], w[2]))
    print("Labels:      +1 = {}, -1 = {}".format(pl, ml))
    print("Predictions: +1 = {}, -1 = {}".format(pp, mp))
    print("Accuracy: {:5.3}%".format(100 * (1 - np.abs(pp - pl) / n_dots)))

    test_plot(test_data, w)

    return None


if __name__ == '__main__':
    main()
