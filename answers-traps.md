# Question1
a0 hold x  
a1 hold 13  
a2 hold 12  
# Question2
从代码li a1,12可以看出，main直接计算出了结果并储存
# Question 3
address 628 is printf
# Question 4
0x38(pc+4)
# Question 5
He110 World  57616的16进制为e110 ,后面的i会转换成字符指针然后因为是小端，所以00 64 6c 72。若为大端则i应改为0x726c6400
# Question 6
输出寄存器a2里的值，因为根据之前ASM代码  printf("%d %d\n", f(8)+1, 13);  
  24:	4635                	li	a2,13  
  26:	45b1                	li	a1,12  
  说明printf第二个参数调用a2寄存器里的（第一个参数调用a1寄存器）所以对Q5的代码也适用