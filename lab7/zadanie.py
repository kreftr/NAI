# -*- coding: utf-8 -*-
"""
Created on Sun Nov 29 16:31:10 2020

@author: Rafal
"""

import random 
import sys




def generate_number(min_value, max_value):
    rand = random.uniform(min_value, max_value)
    return float(format(rand, '.1f'))


def beale_function(x, y):
    return (1.5 - x + x*y)**2 + (2.25 - x + x*y**2)**2 + (2.625 - x + x*y**3)**2


def booth_function(x, y):
    return (x + 2*y - 7)**2 + (2*x + y - 5)**2


def matyas_function(x, y):
    return 0.26*(x**2 + y**2) - 0.48*x*y 


def form_result(best):
    best[0] = float(format(best[0], '.1f'))
    best[1] = float(format(best[1], '.1f'))
    best[2] = float(format(best[2], '.2f'))
    return best


def neighbour():
    x = round(random.uniform(-0.1, 0.1),1)
    y = round(random.uniform(-0.1, 0.1),1)
    return x,y


def hill_climbing(function, x_domain, y_domain, iterations):
    
    x = generate_number(x_domain[0],x_domain[1])
    y = generate_number(y_domain[0],y_domain[1])
    print("[{}, {}] -> {}".format(x,y,function(x, y)))
    best = [x, y, function(x,y)]

    for i in range(iterations):
        
        n = neighbour()
        
        if x + n[0] > x_domain[0] and x + n[0] < x_domain[1] and y + n[1] > y_domain[0] and y + n[1] < y_domain[1]:
            if function(x + n[0], y + n[1]) < function(x, y):
                x = x + n[0]
                y = y + n[1]
                best = [x, y, function(x, y)]
                
    
    return form_result(best)



args = list(sys.argv)


def choose_function(x):
    return {
        'beale': beale_function,
        'booth': booth_function,
        'matyas': matyas_function
    }[x]

function = choose_function(args[1])
x_domain = [float(args[2]),float(args[3])]
y_domain = [float(args[4]),float(args[5])]
iterations = int(args[6])


res = hill_climbing(function, x_domain, y_domain, iterations)

print("[{}, {}] -> {}".format(res[0],res[1],res[2]))

    
