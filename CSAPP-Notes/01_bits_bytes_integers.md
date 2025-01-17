# Bits, Bytes and Integers

只有ENIAC使用十进制进行算术运算    
    
二进制 模拟信号 设定阈值可以起到类似降噪抗干扰的作用    

隐式表示数值集合 如：01000001 {0, 6}    
> & -> Intersection     
> | -> Union    
> ^ -> Symmetric difference    
> ~ -> Complement    
    
     
## &&, ||, !    
+ View 0 as "false"
+ Anything nonzero as "true"
+ Always return 0 or 1
+ **Early termination**
     
### Examples(char data type)
+ !0x41 -> 0x00
+ !0x00 -> 0x01
+ !!0x41 -> 0x01    
   
+ 0x96 && 0x55 -> 0x01
+ 0x96 || 0x55 -> 0x01
+ p && \*p (used to avoid null pointer accesss)

## Shift Operations
### Left Shift: x \<\< y
+ Fill with 0's on right   
+ Throw away extra bit on left   
### Right Shift: x >> y
+ Throw away extra bit on right   
+ **Logical shift**: Fill with 0's on left
+ **Arithmetic shift**: Replicate most significant bit on left
### Undefined Behavior
+ Shift amount < 0 or >= word size(different on different machines)   
    
## Unsigned & Sighed Numeric Values
### Equivalence
+ Same encodings for nonnegative values
### Uniqueness
+ Every bit pattern represents unique integer value
+ Each representable integer has unique bit encoding

### Casting Surprises
If there is a mix of unsigned and signed in single expression,   
**signed values implicitly cast to unsigned**    
Including comparison operations    

### Tmin = -Tmax - 1
### Umax = 2\*Tmax + 1
	Tmax = 01111111
	2*Tmax = 11111110
	Umax = 11111111
### An example
> for (i = n-1; i >= 0; i++) {     
>     cout << a[i] << endl;    	
> }    
> if i is unsigned, loop will never end    
> sizeof() function return unsigned    

## Sign Extension
### Task:
+ Given w-bit signed integer x
+ Convert it to w+k-bit integer with same value
### Rule:
+ Make k copies of sigh bit
+ *C automatically performs sign extension*

### Expanding Basic Rules
+ Unsigned: zeros added
+ Signed: sign extension
+ Both yield expected result

### Truncating Basic Rules
+ Unsigned/signed: bits are truncated
+ Result reinterpreted
+ Unsigned: mod operation
+ Signed: similar to mod
+ For small numbers yields expected behavior
