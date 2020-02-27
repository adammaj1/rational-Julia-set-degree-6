


Julia set of rational function f(z)=z^2(3 − z^4 )/2

![image](s600.png)


see figure 4 on the page 19 in paper "ON THURSTON’S PULLBACK MAP" by XAVIER BUFF, ADAM EPSTEIN, SARAH KOCH, AND KEVIN PILGRIM

basin 		| test 		| color
--------------- | --------------|---------------------------------------------------------------------------------
basin of attration to infinity | bailout or escape test for z=infinity : abs(zn) > ER | iColorOfExterior = 245
basin to attraction to z=0 | test for falling into finite attractor z=1 : abs( zn - 1) < AR   | iColorsOfInterior[1]=230
basin of attraction to z=1 | test for falling into finite attractor z=0 :  abs(zn) < AR   | iColorsOfInterior[0]=200




# Files
* [r.c](r.c)

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
