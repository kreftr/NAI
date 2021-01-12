class Neuron:

    def __init__(self, id, inputs, weights, bias, activation):
        self.id = id
        self.inputs = inputs
        self.weights = weights
        self.bias = bias
        self.activation = activation

    def sum(self):
        sum = 0
        for i in range(len(self.inputs)):
            sum += self.inputs[i] * self.weights[i]

        return sum + self.bias

    def activation_function(self):
        return self.activation(self.sum())

    def change_input(self, inputs):
        self.inputs = inputs

    def print(self):
        return "Id: {}   Sum: {}   Activation: {} ".format(self.id, self.sum(), self.activation_function())
