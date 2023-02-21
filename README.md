# Point Set Polygonization

**Problem:** Polygonization of a point-set **S** with **n** points in $\mathbf{R}^2$, and calculation of the area of the polygon (+ [Pick's theorem](https://en.wikipedia.org/wiki/Pick%27s_theorem)).

The implemented algorithms are based on the following papers:  
- Incremental algorithm from [1]
- Based on Convex Hull algorithm from [2]
- Onion algorithm from [3]

A detailed report of this project it is provided in the `Report.pdf` file.

## Usage

**Dependency:** Make sure you have CGAL installed, or download it from [here](https://www.cgal.org/download.html).

Go to `build/` directory and run:

```
$ cmake .. && make
$ ./to_polygon -i <input file> -o <output file> 
  -algorithm <incremental/convex_hull/onion> 
  -initialization <1a/1b/2a/2b | only in incremental> 
  -edge_selection <1 to 3 | except onion> 
  -onion_initialization <1 to 5 | only in onion>
```

## References

[1]: Taranilla, M.T., Gagliardi, E.O., & Peñalver, G.H. (2011). [Approaching minimum area polygonization.](https://oa.upm.es/19287/1/INVE_MEM_2011_121744.pdf)  
[2]: Zhang H, Zhao Q, Li Y (2020) Generation of simple polygons from ordered points using an iterative insertion algorithm. PLOS ONE 15(3): e0230342. https://doi.org/10.1371/journal.pone.0230342  
[3]: Manuel Abellanas, Jesús García, Gregorio Hernández-Peñalver, Ferrán Hurtado, Oriol Serra, Jorge Urrutia,
Onion polygonizations, Information Processing Letters, Volume 57, Issue 3, 1996, Pages 165-173, ISSN 0020-0190, https://doi.org/10.1016/0020-0190(95)00193-X.

## Authors
* [Kyriakopoulos Dimitrios](https://github.com/dimitrskpl)<sup>*</sup>  
* [Poulis Angelos](https://github.com/angelosps)<sup>*</sup>  
* [Rontogiannis Dimitrios](https://github.com/rondojim)<sup>*</sup>


<sup>*</sup>_Equal contribution._
