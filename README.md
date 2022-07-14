# Sandbox C++ simulation

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)

This is my c++ sanbox simulation made from scratch using only the SFML library for rendering.

Below you will see a list of custom materials and propreties assiociated ith them.


## Materials

|Material|Liquid|Solid (no gravity)|Sand-like|Settable on fire|Evaporates with heat|Corrodible|Can generate heat|Water soluble|
|:-------|:----:|:----------------:|:-------:|:--------------:|:------------------:|:--------:|:---------------:|:-----------:|
|Sand    |      |                  |    ✅   |                |                    |    ✅    |                 |             |
|Water   |  ✅  |                  |         |                |         ✅         |          |                 |             |
|Stone   |      |        ✅        |         |                |                    |    ✅    |                 |             |
|Wood    |      |        ✅        |         |        ✅      |                   |      ✅   |        ✅      |             |
|Salt    |      |                  |   ✅    |                |                    |    ✅    |                 |      ✅     |
|Oil     |  ✅  |                  |         |       ✅       |                    |   ✅     |        ✅       |             |
|Lava    |  ✅  |                  |         |                |                    |          |       ✅        |             |
|Coal    |      |                  |    ✅   |       ✅       |                    |    ✅    |       ✅        |             |
|Snow    |      |                  |    ✅   |                |         ✅         |          |                 |      ✅     |
|Ice     |      |        ✅        |         |                |         ✅         |          |                 |      ✅     |
|Acid    |  ✅  |                  |         |                |                    |          |                 |             |
|Bedrock |      |       ✅         |         |                |                    |          |                 |             |



**special elements :**

- air : empty cell without update.

- fire : air with fire set to on, can propagate heat but won't move or evolve.

- steam : generated when water is heated, float up and after a moment coldowns down back to water and raindrops.

- saltwater : produced by dissolving salt in water, will generate steam and salt cell when heated.



## Examples

<img title="" src="img/lava.gif" alt="Lava flow simulation" width="500">

Lava flow and cooldown into stone.

<img title="" src="img/water_salt.gif" alt="Water dissolving salt" width="500">

Built water circuit to dissolve salt.

<img title="" src="img/oil_coal_wood_acid_vapor.gif" alt="Example of burning elements, and acid destroying stone" width="500">

Oil, wood and coal can be set to fire, and propagate heat, water turn into steam.

Acid destroys stone and organic matter.

<img title="" src="img/ice_snow.gif" alt="Example of ice turning into steam due to the lava" width="500">

Snow and ice melt contact with hot materials such as lava, and turn into water then steam.

## External references :

Some external references I'm using to build this project : 

- [John Jackson's video about Noita's sand simulation](https://www.youtube.com/watch?v=VLZjd_Y1gJ8)

- [MARF's video about this kind of simulation using a Cellular Automata](youtube.com/watch?v=5Ka3tbbT-9E)
