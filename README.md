# Sandbox C++ simulation

This is my c++ sanbox simulation made from scratch using only the SFML library for rendering.

Creating custom physics to make different materials with different specs in more detail below.

some ref I'm using : youtube.com/watch?v=VLZjd_Y1gJ8

                                    youtube.com/watch?v=5Ka3tbbT-9E

# Materials :

- sand 🟡 🟩

- water 🔵 🟦

- stone 🟤 🟩

- wood 🟤 🟥 🟧 🟩

- salt 🟡 ⬜ 🟩

- oil 🔵 🟥 🟧 🟩

- lava 🔵 🟧

- coal 🟡 🟥 🟧 🟩

- snow 🟡 🟦 ⬜

- ice 🟤 🟦 ⬜

- acid 🔵

- bedrock 🟤

- fire 🔺 🟧

- air🔺

- steam🔺

- salt-water 🔺

**legend :**

- 🔵 : move like a liquid

- 🟤 : solid, dont move by gravity

- 🟡 : move like semi-solid or sand

- 🔺 : special element with unique properties

- 🟥 : can be set on fire and burned

- 🟦 : can evaporate with heat

- 🟩 : can be corroded by acid

- 🟧 : can generate heat and make fire

- ⬜ : can dissolve in water



**special elements :**

- air : empty cell without update

- fire : air with fire set to on, and can propagate heat but dont move or evolve

- steam : is generated when water are heated and go up, after a moment it coldown and come back to water form and raindrop.

- salt-water : produce by dissolving salt in water, when heated it generate steam and salt cell.



**example :**

<img title="" src="img/lava.gif" alt="" width="500">

Lava flow and cooldown into stone.

<img title="" src="img/water_salte.gif" alt="" width="500">

Built water circuit to dissolve salt.

<img title="" src="img/oil_coal_wood_acid_vapor.gif" alt="" width="500">

Oil, wood and coal can be set to fire, and propagate heat, water turn into steam.

Acid destroys stone and organic matter.

<img title="" src="img/ice_snow.gif" alt="" width="500">

Snow and ice melt contact with hot materials such as lava, and turn into water then steam.
