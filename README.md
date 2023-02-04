# Point Set Polygonization

**Problem:** Polygonization of a point-set **S** with **n** points in $\mathbf{R}^2$, and calculation of the area of the polygon (+ [Pick's theorem](https://en.wikipedia.org/wiki/Pick%27s_theorem)).

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

## Authors
* [Kyriakopoulos Dimitrios](https://github.com/dimitrskpl)<sup>*</sup>  
* [Poulis Angelos](https://github.com/angelosps)<sup>*</sup>  
* [Rontogiannis Dimitrios](https://github.com/rondojim)<sup>*</sup>


<sup>*</sup>_Equal contribution._
