```
 .d8888b.  8888888b.           888 888                      888    
d88P  Y88b 888  'Y88b          888 888                      888    
888    888 888    888          888 888                      888    
888        888    888  .d88b.  888 88888b.  888d888 .d88b.  888888 
888        888    888 d8P  Y8b 888 888 '88b 888P'  d88''88b 888    
888    888 888    888 88888888 888 888  888 888    888  888 888    
Y88b  d88P 888  .d88P Y8b.     888 888 d88P 888    Y88..88P Y88b.  
 'Y8888P'  8888888P'   'Y8888  888 88888P'  888     'Y88P'   'Y888 
```

Logo created with [TAAG](http://www.patorjk.com/software/taag/).

# A command-line ncurses Mandelbrot Set visualizer written in C. 

### [Here's a quick demo.](https://twitter.com/lbalvarenga55/status/1209584156298170369)

## Compilation

In a Linux shell, clone the repository with

```bash
$: git clone https://github.com/lukeathedev/cdelbrot.git
```

then

```bash
$: make run
```

or

```bash
$: make
$: ./mandebrot
```

to compile and run the program.

## Tips & tricks

```    
                                        . .            
                                      ..+..            
                                      .####.           
                               .#  #.-..##.....        
                               -##################-.   
                             .####################.    
                 ... ..     .#######################-  
                 ..+######...#######################.  
              ....#################################.   
  ###############################################.     
              ....#################################.   
                 ..+######...#######################.  
                 ... ..     .#######################-  
                             .####################.    
                               -##################-.   
                               .#  #.-..##.....        
                                      .####.           
                                      ..+..            
                                        . .            
```

+ Decreasing the font size yields a much sharper and nicer looking fractal (at the cost of execution time).
+ Adjusting <b>RESOLUTION</b> in <i>src/mandebrot.c</i> to a lower or higher value can help with image quality.
+ Adjusting <b>SENSITIVITY</b> in <i>src/mandelbrot.c</i> to a lower value will reduce the amount of movement between frames.
+ Mess around in the source code! Change parameters, modify functions, add features!