import neuron

activation = lambda x: 1 if x > 0 else 0

inputs = [0, 1]

neuron1 = neuron.Neuron(1, inputs, [1, 1], -0.5, activation)
neuron2 = neuron.Neuron(2, inputs, [-1, -1], 1.5, activation)
neuron3 = neuron.Neuron(3, [neuron1.activation_function(), neuron2.activation_function()], [1, 1], -1.5, activation)

print(neuron1.print())
print(neuron2.print())
print(neuron3.print())
print(neuron3.activation_function())

inputs = [0, 0]

neuron1.change_input(inputs)
neuron2.change_input(inputs)
neuron3.change_input([neuron1.activation_function(), neuron2.activation_function()])

print(neuron1.print())
print(neuron2.print())
print(neuron3.print())
print(neuron3.activation_function())
