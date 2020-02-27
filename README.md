


Julia set of rational function f(z)=z^2(3 − z^4 )/2

![image](s600.png)

See also [commons](https://commons.wikimedia.org/wiki/File:Julia_set_of_rational_function_f(z)%3Dz%5E2(3_%E2%88%92_z%5E4_)_over_2.png)


see figure 4 on the page 19 in paper "ON THURSTON’S PULLBACK MAP" by XAVIER BUFF, ADAM EPSTEIN, SARAH KOCH, AND KEVIN PILGRIM



# algorithm

There are 3 fixed points
* z = infinity
* 2 fixed points 
  * z = 0
  * z = 1
  
Program test each point z  to which basin it falls after some iterations 
  

basin 		| test 		| color
--------------- | --------------|---------------------------------------------------------------------------------
basin of attration to infinity | bailout or escape test for z=infinity : abs(zn) > ER | iColorOfExterior = 245
basin of attraction to z=0 | test for falling into finite attractor z=1 : abs( zn - 1) < AR   | iColorsOfInterior[1]=230
basin of attraction to z=1 | test for falling into finite attractor z=0 :  abs(zn) < AR   | iColorsOfInterior[0]=200

where:
* zn = f^n(z0)
* AR = Attraction Radius = dMaxDistance2fixed. It is a radius of circle with center in the fixed point  and inside immediate basin of attraction 
* ER = Escape Radius  = 2.0; ( Escape Radius for bailout test )


# Files
* [r.c](r.c) -  c console progam 

#Licence
 [GNU GENERAL PUBLIC LICENSE Version 2, June 1991](LICENCE)


# Git

```git
git add .
git commit -m "Initial commit"
git push -u origin master
```


clone: 

```
  git clone https://github.com/adammaj1/rational-Julia-set-degree-6.git
```


local repo
```
/media/a/.../home/a/c/julia/rational/rational6d
``` 

## markdown
* [github guide: mastering-markdown/](https://guides.github.com/features/mastering-markdown/)
* [github help: writing-on-github/basic-writing-and-formatting-syntax](https://help.github.com/en/github/writing-on-github/basic-writing-and-formatting-syntax)
* [mMarkdown-Cheatsheet by Adam Pritchard](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
* [README-Template.md by Billie Thompson](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [how-to-show-math-equations-in-general-githubs-markdown, not-githubs-blog](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog)
