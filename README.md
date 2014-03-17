## What is Malva?

Malva is a fork of the MALLBA project. MALLBA is an effort to develop, in an integrated way, a
library of skeletons for combinatorial optimization (including exact, heuristic and hybrid methods)
that can deal with parallelism in a user-friendly and, at the same time, efficient manner.
Its three target environments are sequential computers, LANs of workstations and WANs. The main features of MALLBA are:

* Integration of all the skeletons under the same design principles.
* Facility to switch from sequential to parallel optimization engines.
By providing sequential implementations users obtain parallel implementations.
* Cooperation between engines makes possible to provide more powerful hybrid engines.
* Ready to use on commodity machines.
* Flexible and extensible software architecture. New skeletons can easily be added, alternative communication layers
can be used, etc.

## How to install and compile Malva

First of all, you have to install MPICH library. Get the instructions here: http://www.mpich.org/. In OSX, you can just
type:

    brew install mpich --disable-fortran

Disabling Fortran is needed since we are working with C++ instead of Fortran.

After that, you have to clone this repository using `git clone https://github.com/gabrielfagundez/malva`, and configure Malva modifying the `environment` file.

Then, execute the following command:

    make all

Your Malva directory should contain the following content after the make:

```
         -rw-r--r--    1 user   users           378 sep 24 09:39 Makefile
         drwxr-xr-x    4 user   users          4096 sep 24 09:48 ProblemInstances
         -rw-r--r--    1 user   users           345 sep 24 16:29 environment
         drwxr-xr-x    2 user   users          4096 sep 24 16:43 inc
         drwxr-xr-x    2 user   users          4096 sep 24 17:49 lib
         drwxr-xr-x    5 user   users          4096 sep 24 09:41 rep
         drwxr-xr-x    2 user   users          4096 sep 24 17:49 src
```

## Testing the installation

Go to `MALLBA_DIR/rep_new/CHC` or `MALLBA_DIR/rep_new/GA` and execute `make SEQ` or `make LAN`. If you have in your console
the result of the execution, everything looks great!

## Important notes

The only new fully-functional algorithms are the GA and the CHC.

## Architecture

Mallba skeletons are based on the separation of two concepts: the concrete problem to be solved and the general
resolution method to be used. They can be seen as generic templates that just need to be instantiated with the
features of a problem in order to solve it. All features related to the selected generic resolution method and
its interaction with the concrete problem are implemented by the skeleton. While the particular features related
to the problem must be given by the user, the knowledge to parallelize the execution of the resolution method is
implemented in the skeleton, so that users do not need to deal with parallelism issues.

The design of the Mallba library focuses on easy to use skeletons and general and efficient implementations.
To achieve both objectives, the C++ programming language was selected due to its high level, modularity,
flexibility and efficiency features. We have reduced to a minimum the use of inheritance and virtual methods in
order to provide better efficiency and ease of use. To instantiate most problems, a basic knowledge of C++ is enough,
and only sequential code without side effects is needed.

Skeletons are implemented by a set of required and provided C++ classes that represent an abstraction of the entities
participating in the resolution method. The provided classes implement internal aspects of the skeleton in a
problem-independent way. The required classes specify information and behavior related to the problem.
This conceptual separation allows us to define required classes with a fixed interface but without any implementation,
so that provided classes can use required classes in a generic way.

More specifically, each skeleton includes the Problem and Solution required classes, that encapsulate the
problem-dependent entities needed by the resolution method. The Problem class abstracts the features of the
problem that are relevant to the selected optimization method. The Solution class abstracts the features of the
feasible solutions that are relevant to the selected resolution method. Depending on the skeleton, other classes
may be required. On the other hand, each skeleton offers two provided classes: Solver and SetUpParams. The former
abstracts the selected resolution method. The later contains the setup parameters needed to perform the execution
(e.g. number of iterations, number of independent runs, parameters guiding the search, etc.). The Solver class provides
methods to run the resolution scheme and methods to consult its progress or change its state. The only information the
solver needs is an instance of the problem to solve and the setup parameters. In order to enable an skeleton to have
different solver engines, the Solver class defines a unique interface and provides several subclasses that provide
different sequential and parallel implementations (Solver_Seq, Solver_Lan and Solver_Wan). In Fig. 1 is shown the
common design of Mallba skeletons.

![architecture](https://raw.github.com/gabrielfagundez/malva/master/uml-comun.gif)

## Implementation

The implementation of each skeleton is contained in three files:

* <skeleton>.hh: The file containing the definition of all classes (provides and requires).
* <skeleton>.pro.cc: The file containing the source code of the classes needed for the  internal implementation of the method.
* <skeleton>.req.cc: The source file where all the required classes will be implemented.

In additional, the user must configure the method parameters in the file <skeleton>.cfg.

## Supported Algorithms

### Genetic Algorithm

A Genetic Algorithm is an evolutionary computation technique inspired by the principles of natural selection to search
a solution space. It evolves a population of individuals encoded as chromosomes by creating new generations of
offsprings through an iterative process until some convergence criteria or conditions are met. The best chromosome
generated is then decoded, providing the corresponding solution. The underlying reproduction process is mainly aimed
at improving the fitness of individuals, a measure of profit, utility or goodness to be maximized (or minimized) while
exploring the solution space. The algorithm applies stochastic operators such as selection, crossover, and mutation,
on an initially random population in order to compute a new generation of individuals. The whole process is sketched
in the following figure.

```
 1 t = 0
 2 initialize P(t)
 3 evaluate structures in P(t)
 4 while not end do
 5    t = t + 1
 6    select C(t) from P(t-1)
 7    recombine structures in C(t) forming C'(t)
 8    mutate structures in C'(t) forming C''(t)
 9    evaluate structures in C''(t)
10    replace P(t) from C''(t) and/or P(t-1)
```

It can be seen that the algorithm comprises three major stages: selection, reproduction and replacement. During the
selection stage, a temporary population is created in which the fittest individuals (those corresponding to the best
solutions contained in the population) have a higher number of instances than those less fit (natural selection). The
reproductive operators are applied to the individuals in this population yielding a new population. Finally, individuals
of the original population are substituted by the new created individuals. This replacement usually tries to keep the
best individuals deleting the worst ones. The whole process is repeated until a certain termination criterion is
achieved (usually after a given number of iterations).

The Genetic Algorithm skeleton (GA) requires the classes:

* Problem
* Solution
* Crossover
* Mutation

The class Problem corresponds to the definition of a problem instance. The skeleton filler must provide a complete
definition of this class.

The class Solution corresponds to the definition of a solution (feasible or not) of a problem instance. The skeleton
filler must provide a complete definition of the class Solution.

The class Crossover corresponds to the definition of a crossover operator. The skeleton filler must provide a complete
definition of this class.

And finally, the class Mutation corresponds to the definition of a mutation operator. The skeleton filler must provide
a complete definition of this class.

In adition, the user must configure the following algorithm parameters (in file GA.cfg):

* number of independent runs.
* number of generations.
* size of population.
* size of offsprings in each generation.
* replace mode (if replaces parents for offsprings, or only offsprings may be new parents).
* Selection operators parameters (selection of parents and selection of offsprings parameters).
* Intra operators parameters (crossover and mutation parameters).
* Inter operators (operators to apply between sub-populations) parameters: operator number, operator rate, number of individuals and selection of individual to send and replace.
* Parallel Configuracion: interval of generation to refresh global state, running mode (synchronized or asyncronized) and interval of generations to check solutions from other populations.

There are several basic steps to running a problem solve with GA skeleton:

* Change to the problem directory

    `cd Mallba/rep/GA/problem`

* Compile skeleton.

    `make`

* Configure algorithm parameters (GA.cfg file)

* Run problem:
Sequential Version:

```
make SEQ
     or
MainSeq GA.cfg_path instance_path res_file_path
```

Parallel Version:

Configure Config.cfg file.
Configure pgfileLan (or pgfileWan) : machines where we run the program.
Run

```
make LAN
    or
make WAN
```

### CHC

A CHC is a non-traditional GA which combines a conservative selection strategy (that always preserves the best
individuals found so far) with a highly disruptive recombination (HUX) that produces offsprings that are maximally
different from their two parents. The traditional though of preferring a recombination operator with a low disrupting
properties may not hold when such a conservative selection strategy is used. On the contrary, certain highly disruptive
crossover operator provide more effective search in many problems, which represents the core idea behind the

CHC search method. This algorithm introduce a new bias against mating individuals who are too similar (incest
prevention). Mutation is not performed, instead, a restart process re-introduces diversity whenever convergence is
detected.

```
 1 t = 0
 2 initialize P(t)
 3 evaluate structures in P(t)
 4 while not end do
 5    t = t + 1
 6    select: C(t) = P(t-1)
 7    recombine: C'(t) = 'incest prevention' + HUX(C'(t))
 8    evaluate structures in C'(t)
 9    replace P(t) from C''(t) and P(t-1)
10    if convergence(P(t))
11        diverge P(t)
```

The CHC method skeleton (CHC) requires the classes:

* Problem
* Solution

The class Problem corresponds to the definition of a problem instance. The skeleton filler must provide a complete
definition of this class.

And finally, the class Solution corresponds to the definition of a solution (feasible or not) of a problem instance.
The skeleton filler must provide a complete definition of the class Solution.

In adition, the user must configure the following algorithm parameters (in file CHC.cfg):

* number of independent runs.
* number of generations.
* size of population.
* Selection operator parameters (selection of new parents and the diverge method that the algorithm uses whenever the
convergence is detected).
* Intra operators parameters (crossover and mutation parameters).
* Inter operators (operators to apply between sub-populations) parameters: operator number, operator rate, number of
individuals and selection of individual to send and replace.
* Parallel Configuracion: interval of generation to refresh global state, running mode (synchronized or asyncronized)
and interval of generations to check solutions from other populations.

There are several basic steps to running a problem solve with CHC skeleton:

* Change to the problem directory

    `cd Mallba/rep/CHC/problem`

* Compile skeleton

    `make`

* Configure algorithm parameters (CHC.cfg file)

* Run problem:

Sequential Version:

```
make SEQ
     or
MainSeq GA.cfg_path instance_path res_file_path
```

Parallel Version:

Configure Config.cfg file.
Configure pgfileLan (or pgfileWan) : machines where we run the program.
Run

```
make LAN
    or
make WAN
```
