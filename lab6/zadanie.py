# -*- coding: utf-8 -*-
"""
Created on Mon Nov 23 21:57:13 2020

@author: Rafal
"""
import math
import random 


def generate_number(min_value, max_value):
   #return random.randint(min_value, max_value)
    rand = random.uniform(min_value, max_value)
    return float(format(rand, '.1f'))



def optimalization_function(function, domain_x_max, domain_x_min, domain_y_max, domain_y_min, iterations):
    
    best = []
    
    for i in range(iterations):
        
        x = generate_number(domain_x_min, domain_x_max)
        y = generate_number(domain_y_min, domain_y_max)
        
        
        new = function(x,y)
       
        
        if i == 0:
            best.append(float(format(new, '.2f')))
            best.append(x)
            best.append(y)
        elif new < best[0]:
            best.clear()
            best.append(float(format(new, '.2f')))
            best.append(x)
            best.append(y)
            #print("[{}] x: {}   y: {}   f: {}   best: {}".format(i,x,y,format(new, '.1f'), format(best[0], '.1f')))
         
    return best



def beale_function(x, y):
    return (1.5 - x + x*y)**2 + (2.25 - x + x*y**2)**2 + (2.625 - x + x*y**3)**2


def booth_function(x, y):
    return (x + 2*y - 7)**2 + (2*x + y - 5)**2


def matyas_function(x, y):
    return 0.26*(x**2 + y**2) - 0.48*x*y 


result = optimalization_function(beale_function, 10, -4.5, 10, -4.5 , 10000) 
print("[{}, {}] -> {}".format(result[1],result[2], result[0]))

print(beale_function(3, 0.5))
