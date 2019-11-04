
# coding: utf-8

# In[1]:


get_ipython().run_line_magic('matplotlib', 'inline')
import numpy as np
import matplotlib.pyplot as plt


# In[2]:


from math import *
def f(x):
    if x==0:
        return 0
    else :
        return sqrt(x)*log(x)


# In[3]:


def get_n(a, b, h):
    n = int((b-a)/h)
    if n % 2 == 1:
        n = n + 1               
    return n


# In[4]:


def generate_data(a, b, h):
    n = get_n(a, b, h)
    data = []
    x = a
    for i in range(n):
        data.append(f(x))
        x += h
    return data


# In[5]:


def simpson(data, h, n):
    sum = data[0] + data[n-1]
    for i in range(2, n):
        if i % 2 == 0:
            sum += 4 * data[i-1]
        else:
            sum += 2 * data[i-1]
    sum *= h / 3.0
    return sum


# In[12]:


a = 0.0
b = 1.0
h = []
simp = []
error = []
for i in range(1, 10):
    h.append(i*pow(10, -8))
for i in range(9):
    n = get_n(a, b, h[i])
    data = generate_data(a, b, h[i])
    simp.append(simpson(data, h[i], n)) 
    error.append(simp[i]-4/9)
    print(simp[i])
    print("\n")
plt.plot(h, simp)
plt.xlabel("h")
plt.ylabel("simpson")


# In[14]:


for i in range(9):
    error[i]+=8/9
plt.plot(h, error)
plt.xlabel("h")
plt.ylabel("error")

