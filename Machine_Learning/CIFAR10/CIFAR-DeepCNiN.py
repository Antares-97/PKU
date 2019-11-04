from __future__ import division

import cifar10_input
import tensorflow as tf
import numpy as np
import time
from datetime import datetime

now = datetime.utcnow().strftime("%Y%m%d%H%M%S")
root_logdir = "tf_logs"
logdir = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw2/CIFAR/CIFAR-CNN-simplified/log_dir"


def variable_with_weight_loss(shape, stddev, wl):
    '''
    使用tf.truncated_normal截断的正态分布来初始化权重,这里给weight加一个L2的loss.
    我们使用wl控制L2 loss的大小,使用tf.nn.l2_loss计算weight的L2 loss.
    再使用tf.multiply让L2 loss乘wl,得到最后的weight loss,最后将weight loss添加到一个collection.便于后期优化
    :param shape:
    :param stddev:
    :param wl:
    :return:
    '''
    var = tf.Variable(tf.truncated_normal(shape, stddev=stddev))
    if wl is not None:
        weight_loss = tf.multiply(tf.nn.l2_loss(var), wl, name='weight_loss')
        tf.add_to_collection('losses', weight_loss)
    return var


def loss(logits, labels):
    '''
    使用tf.nn.sparse_softmax_cross_entropy_with_logits将softmax和cross_entropy_loss计算合在一起
    并计算cross_entropy的均值添加到losses集合.以便于后面输出所有losses
    :param logits:
    :param labels:
    :return:
    '''
    labels = tf.cast(labels, tf.int64)
    cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=logits,
                                                                   labels=labels, name='cross_entropy_per_example')
    cross_entropy_mean = tf.reduce_mean(cross_entropy, name='cross_entropy')
    tf.add_to_collection('losses', cross_entropy_mean)

    return tf.add_n(tf.get_collection('losses'), name='total_loss')


'''
使用cifar10_input类中的disorted_inputs函数产生训练数据,产生的数据是已经封装好的Tensor,每次会产生batch_size个
这个函数已经对图片数据做了增强操作(随机水平翻转/剪切/随机对比度等)
同时因为对图像处理需要耗费大量计算资源,该函数使用了16个独立的线程来加速任务,
函数内部会产生线程池,使用会通过TensorFlow queue进行调度
'''

batch_size = 128
data_dir = '/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Machine Learning/hw2/CIFAR/cifar-10-batches-bin'

with tf.name_scope('graph') as scope:
    images_train, labels_train = cifar10_input.distorted_inputs(data_dir=data_dir, batch_size=batch_size)
    images_test, labels_test = cifar10_input.inputs(eval_data=True, data_dir=data_dir, batch_size=batch_size)

    image_holder = tf.placeholder(tf.float32, [batch_size, 24, 24, 3])  # 128张图
    label_holder = tf.placeholder(tf.int32, [batch_size])

    '''
    This is based on the dissertation of ""
    We developed a network structure of DeepCNiN(5,100)
    which is structured like:
    input-100C3(conv1)-drop1-MP2(pool1)-100C1(nin1)-
         -200C2(conv2)-drop2-MP2(pool2)-200C1(nin2)-
         -300C2(conv3)-drop3-MP2(pool3)-300C1(nin3)-
         -400C2(conv4)-drop4-MP2(pool4)-400C1(nin4)-
         -500C2(conv5)-drop5-MP2(pool5)-500C1(nin5)-
         -600C2(conv6)-drop6-600C1(nin6)-fc7-drop_fc7-fc8-drop_fc8-softmax

    dropout only applied to the convolutional layers
    rather than NiN layers
    the keep_proportions were: 1.0, 1.0, 0.9, 0.8, 0.7, 0.6, and 0.5 for fc layer
    activation function: leaky ReLU
    '''
    keep_proportion_conv1 = tf.placeholder(tf.float32)
    keep_proportion_conv2 = tf.placeholder(tf.float32)
    keep_proportion_conv3 = tf.placeholder(tf.float32)
    keep_proportion_conv4 = tf.placeholder(tf.float32)
    keep_proportion_conv5 = tf.placeholder(tf.float32)
    keep_proportion_conv6 = tf.placeholder(tf.float32)
    keep_proportion_fc = tf.placeholder(tf.float32)

    # 第一层
    weight1 = variable_with_weight_loss(shape=[3, 3, 3, 100], stddev=5e-2, wl=0.0)
    kernel1 = tf.nn.conv2d(image_holder, weight1, strides=[1, 1, 1, 1], padding='SAME')
    bias1 = tf.Variable(tf.constant(0.0, shape=[100]))  # bias直接初始化为0
    conv1 = tf.nn.relu(tf.nn.bias_add(kernel1, bias1))  # 第一层3x3的卷积核
    drop1 = tf.nn.dropout(conv1, keep_proportion_conv1)  # dropout
    pool1 = tf.nn.max_pool(drop1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')  # 第一层池化
    _weight1 = variable_with_weight_loss(shape=[1, 1, 100, 100], stddev=5e-2, wl=0.0)
    _kernel1 = tf.nn.conv2d(pool1, _weight1, strides=[1, 1, 1, 1], padding='SAME')
    _bias1 = tf.Variable(tf.constant(0.0, shape=[100]))
    nin1 = tf.nn.relu(tf.nn.bias_add(_kernel1, _bias1))  # 第一层 nin

    weight2 = variable_with_weight_loss(shape=[2, 2, 100, 200], stddev=5e-2, wl=0.0)
    kernel2 = tf.nn.conv2d(nin1, weight2, strides=[1, 1, 1, 1], padding='SAME')
    bias2 = tf.Variable(tf.constant(0.1, shape=[200]))
    conv2 = tf.nn.relu(tf.nn.bias_add(kernel2, bias2))
    drop2 = tf.nn.dropout(conv2, keep_proportion_conv2)
    pool2 = tf.nn.max_pool(drop2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    _weight2 = variable_with_weight_loss(shape=[1, 1, 200, 200], stddev=5e-2, wl=0.0)
    _kernel2 = tf.nn.conv2d(pool2, _weight2, strides=[1, 1, 1, 1], padding='SAME')
    _bias2 = tf.Variable(tf.constant(0.0, shape=[200]))
    nin2 = tf.nn.relu(tf.nn.bias_add(_kernel2, _bias2))  # 第二层 nin

    weight3 = variable_with_weight_loss(shape=[2, 2, 200, 300], stddev=5e-2, wl=0.0)
    kernel3 = tf.nn.conv2d(nin2, weight3, strides=[1, 1, 1, 1], padding='SAME')
    bias3 = tf.Variable(tf.constant(0.1, shape=[300]))
    conv3 = tf.nn.relu(tf.nn.bias_add(kernel3, bias3))
    drop3 = tf.nn.dropout(conv3, keep_proportion_conv3)
    pool3 = tf.nn.max_pool(drop3, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    _weight3 = variable_with_weight_loss(shape=[1, 1, 300, 300], stddev=5e-2, wl=0.0)
    _kernel3 = tf.nn.conv2d(pool3, _weight3, strides=[1, 1, 1, 1], padding='SAME')
    _bias3 = tf.Variable(tf.constant(0.0, shape=[300]))
    nin3 = tf.nn.relu(tf.nn.bias_add(_kernel3, _bias3))  # 第三层 nin

    weight4 = variable_with_weight_loss(shape=[2, 2, 300, 400], stddev=5e-2, wl=0.0)
    kernel4 = tf.nn.conv2d(nin3, weight4, strides=[1, 1, 1, 1], padding='SAME')
    bias4 = tf.Variable(tf.constant(0.1, shape=[400]))
    conv4 = tf.nn.relu(tf.nn.bias_add(kernel4, bias4))
    drop4 = tf.nn.dropout(conv4, keep_proportion_conv4)
    pool4 = tf.nn.max_pool(drop4, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    _weight4 = variable_with_weight_loss(shape=[1, 1, 400, 400], stddev=5e-2, wl=0.0)
    _kernel4 = tf.nn.conv2d(pool4, _weight4, strides=[1, 1, 1, 1], padding='SAME')
    _bias4 = tf.Variable(tf.constant(0.0, shape=[400]))
    nin4 = tf.nn.relu(tf.nn.bias_add(_kernel4, _bias4))  # 第四层 nin

    weight5 = variable_with_weight_loss(shape=[2, 2, 400, 500], stddev=5e-2, wl=0.0)
    kernel5 = tf.nn.conv2d(nin4, weight5, strides=[1, 1, 1, 1], padding='SAME')
    bias5 = tf.Variable(tf.constant(0.1, shape=[500]))
    conv5 = tf.nn.relu(tf.nn.bias_add(kernel5, bias5))
    drop5 = tf.nn.dropout(conv5, keep_proportion_conv5)
    pool5 = tf.nn.max_pool(drop5, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    _weight5 = variable_with_weight_loss(shape=[1, 1, 500, 500], stddev=5e-2, wl=0.0)
    _kernel5 = tf.nn.conv2d(pool5, _weight5, strides=[1, 1, 1, 1], padding='SAME')
    _bias5 = tf.Variable(tf.constant(0.0, shape=[500]))
    nin5 = tf.nn.relu(tf.nn.bias_add(_kernel5, _bias5))  # 第五层 nin

    weight6 = variable_with_weight_loss(shape=[2, 2, 500, 600], stddev=5e-2, wl=0.0)
    kernel6 = tf.nn.conv2d(nin5, weight6, strides=[1, 1, 1, 1], padding='SAME')
    bias6 = tf.Variable(tf.constant(0.1, shape=[600]))
    conv6 = tf.nn.relu(tf.nn.bias_add(kernel6, bias6))
    drop6 = tf.nn.dropout(conv6, keep_proportion_conv6)
    pool6 = tf.nn.max_pool(drop6, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    _weight6 = variable_with_weight_loss(shape=[1, 1, 600, 600], stddev=5e-2, wl=0.0)
    _kernel6 = tf.nn.conv2d(pool6, _weight6, strides=[1, 1, 1, 1], padding='SAME')
    _bias6 = tf.Variable(tf.constant(0.0, shape=[600]))
    nin6 = tf.nn.relu(tf.nn.bias_add(_kernel6, _bias6))  # 第六层 nin

    # 第七层 使用全连接层 reshape后获取长度并创建FC1层的权重
    nin6_reshaped = tf.reshape(nin6, [batch_size, -1])
    dim = nin6_reshaped.get_shape()[1].value
    weight7 = variable_with_weight_loss(shape=[dim, 384], stddev=0.04, wl=0.004)  # 这个384可能是瞎设的?
    bias7 = tf.Variable(tf.constant(0.1, shape=[384]))
    local7 = tf.nn.relu(tf.matmul(nin6_reshaped, weight7) + bias7)
    fc7 = tf.nn.dropout(local7, keep_proportion_fc)

    # 第八层 FC2层  节点数减半
    weight8 = variable_with_weight_loss(shape=[384, 192], stddev=0.04, wl=0.004)
    bias8 = tf.Variable(tf.constant(0.1, shape=[192]))
    local8 = tf.nn.relu(tf.matmul(fc7, weight8) + bias8)
    fc8 = tf.nn.dropout(local8, keep_proportion_fc)

    # 最后一层 这层的weight设为正态分布标准差设为上一个FC层的节点数的倒数
    # 这里我们不计算softmax,把softmax放到后面计算
    weight9 = variable_with_weight_loss(shape=[192, 10], stddev=1 / 192.0, wl=0.0)
    bias9 = tf.Variable(tf.constant(0.0, shape=[10]))
    logits = tf.add(tf.matmul(fc8, weight9), bias9)

    # 损失函数为两个带L2正则的FC层和最后的转换层
    # 优化器依旧是AdamOptimizer,学习率是1e-3
    losses = loss(logits, label_holder)
    global_step = tf.Variable(0)  # 相当于global_step,是一个全局变量,在训练完一个批次后自动增加1

    #  学习率使用退化学习率的方法
    # 设置初始学习率为0.001,
    learning_rate = tf.train.exponential_decay(learning_rate=0.001, global_step=global_step,
                                               decay_steps=200, decay_rate=0.9, staircase=True)  # 每200轮之后乘以0.9
    optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(loss=losses, global_step=global_step)
    add_global = global_step.assign_add(1)

    # in_top_k函数求出输出结果中top k的准确率,这里选择输出top1
    top_k_op = tf.nn.in_top_k(logits, label_holder, 1)


if __name__ == '__main__':
    # 创建默认session,初始化变量
    sess = tf.InteractiveSession()
    tf.global_variables_initializer().run()
    merged = tf.summary.merge_all()
    writer = tf.summary.FileWriter(logdir, sess.graph)

    # 启动图片增强线程队列
    tf.train.start_queue_runners()

    max_steps = 23000

    # 训练
    for step in range(max_steps):
        start_time = time.time()
        image_batch, label_batch = sess.run([images_train, labels_train])
        _, _, loss_value = sess.run([optimizer, add_global, losses], feed_dict={image_holder: image_batch,
                                                                                label_holder: label_batch,
                                                                                keep_proportion_conv1: 1.0,
                                                                                keep_proportion_conv2: 1.0,
                                                                                keep_proportion_conv3: 1.0,
                                                                                keep_proportion_conv4: 1.0,
                                                                                keep_proportion_conv5: 1.0,
                                                                                keep_proportion_conv6: 1.0,
                                                                                keep_proportion_fc: 0.5})

        tf.summary.scalar('train_loss', loss_value)
        duration = time.time() - start_time

        if step % 10 == 0:
            examples_per_sec = batch_size / duration
            sec_per_batch = float(duration)

            rs = sess.run(merged)
            writer.add_summary(rs, loss_value)

            format_str = 'step %d,loss=%.2f (%.1f examples/sec; %.3f sec/batch)'
            print(format_str % (step, loss_value, examples_per_sec, sec_per_batch))

    writer.close()

    # 评估模型的准确率,测试集一共有10000个样本
    # 我们先计算大概要多少个batch能测试完所有样本
    num_examples = 10000
    import math

    num_iter = int(math.ceil(num_examples / batch_size))
    true_count = 0
    total_sample_count = num_iter * batch_size  # 除去不够一个batch的
    step = 0
    while step < num_iter:
        image_batch, label_batch = sess.run([images_test, labels_test])
        predictions = sess.run([top_k_op], feed_dict={image_holder: image_batch,
                                                      label_holder: label_batch,
                                                      keep_proportion_conv1: 1.0,
                                                      keep_proportion_conv2: 1.0,
                                                      keep_proportion_conv3: 1.0,
                                                      keep_proportion_conv4: 1.0,
                                                      keep_proportion_conv5: 1.0,
                                                      keep_proportion_conv6: 1.0,
                                                      keep_proportion_fc: 1.0})
        true_count += np.sum(predictions)  # 利用top_k_op计算输出结果
        step += 1

    precision = true_count / total_sample_count
    print('precision @ 1=%.3f' % precision)  # 这里如果输出为0.00 是因为整数/整数  记得要导入float除法