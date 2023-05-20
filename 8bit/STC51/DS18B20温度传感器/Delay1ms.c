void Delay1ms(unsigned int ms)		//@11.0592MHz 延时器
{
	unsigned char i, j;
	while(ms)
  {
  	i = 2;
	  j = 199;
	  do
	  {
		  while (--j);
	  } while (--i);
		ms--;
  }
}