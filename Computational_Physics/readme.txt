此压缩包包含三道计算物理大作业的题目解答和报告,其中T1,T2,T3分别包含第一,第二,第三道作业题的实现代码与相应的结果文件.report.pdf 是三道作业题的解题报告.
T1文件夹内包含:
1.solution.txt 即本题偏微分方程的解
2.data1.mat matlab 数据文件, 将 solution中的数据导入了matlab, 并进行了reshape 操作,还原成网格
3.1/2.pdf matlab 绘制的图片,将数据点画在三维图上. 为了清晰地展示,对 theta 轴进行了一次反转
4.T1.xcodeproj 与 main.cpp 实现代码

T2文件夹内包含:
1.T2.xcodeproj, main.cpp 实现代码
2.A.txt Jacobi 算法迭代处理之后,最终得到的 A 系数矩阵.(应当是个接近于对角的矩阵)
3.energy.txt 能量值,直接取于 A 矩阵的对角线元素.是 A 矩阵的本征值
4.sorted_E.txt energy经排序后升序排列的能级
5.wave_function.txt 即正交矩阵 P 的具体形式,每一列是对应能量的波函数
6.probability.txt 即波函数的模方,也即每一个格点上的分布概率,最后一行是对每一列的求和值,可以验证波函数归一性
7.data2.mat 将上述所有数据导入 matlab 的数据文件
8.sample.pdf 波函数模方绘图示例,任取了某一波函数进行绘图,对应能级为98.7346
9.integral.png 验证每一列波函数(模方)归一

T3文件夹内包含:
1.T3.xcodeproj, main.cpp 实现代码
2.const.m / five_val.m / func.m 用于 matlab 实现五维函数求极值的代码文件
3.result.png 程序运行结果截图
4.matlab.jpeg MATLAB 程序运行结果截图

解题报告: 1500011370_刘浚哲.pdf 内容为三道大作业题目的详细解题报告.