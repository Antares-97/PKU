
# coding: utf-8

# In[39]:


get_ipython().run_line_magic('matplotlib', 'inline')


# In[40]:


import numpy as np
import matplotlib.pyplot as plt


# In[77]:


t=np.linspace(0,1,64)
print(t)


# In[78]:


y=np.ones_like(t)


# In[79]:


yt=np.ones_like(t)
for _ in range(5):
    for i in range(len(yt)):
        yt[i]=np.trapz((t-t[i])*y,t)
    yt+=np.exp(2*t)+(np.exp(2)-1)*t/2-(np.exp(2)+1)/4
    print(np.linalg.norm(yt-y))
    y=yt


# In[80]:


np.linalg.norm(yt)


# In[81]:


plt.plot(t,yt)
plt.plot(t,np.exp(2*t))
plt.show()


# In[82]:


print(yt[0]-np.exp(0))
print(yt[63]-np.exp(2))
print(yt[16]-np.exp(2*0.25))
print(yt[48]-np.exp(2*0.75))

