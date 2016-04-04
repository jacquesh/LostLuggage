# TODO
A reference video for the original Lost Luggage can be found [here](https://www.youtube.com/watch?v=fd75-5xvvaQ).

Note that sub-listing indicates a dependency (so the outer-item is (thought to be) necessary before the inner items can be completed).

Items are not placed in any particular order

## Shorter-term/Less major
- Make the code-style consistent (we have bits where consecutive lines don't even have the same indent width)
- Rewrite bag motion so that it is more continous/elegant (motion system has a number of requirements to satisfy, will warrant some thinking)
    - Add pushers (reference level 7)
    - Add walls/blockers (reference lvl 7)
    - Add slides(naming? Chutes works better for the "teleporter" things. the conveyers that have no direction/are just metal in original Lost Luggage, reference lvl 15)
    - Add unflippable conveyers (reference lvl 16)
    - Add chutes (the teleporter things, reference lvl 21)
    - Add the T-junction direction selectors (reference lvl 28)
    - Add corner conveyers (reference lvl 29)
    - Add perma-enabled pushers (reference lvl 40)
- Add a win condition (IE have it actually check that inactive bags reached their desired bin)
    - Add an actual level sequence (so that when a level is finished, it loads the next one)
- Upgrade the map format to support all the things we want to place on the map. This will almost necessarily make it very non-human-readable, so it will almost certainly warrant a near-complete rewrite of the level-loading code

## Longer-term/More major
- New/Non-cloned mechanics! (Bags changing colour, bags being spawned over time, "Hi-speed" conveyers, "fragile" bags that can't go on the fast conveyers/through chutes, etc)
- Add some actual graphics (so that we're not just using squares)
- Add some sound (I've never had sound in dge so it'll need to be added, I'll look into a library for it, possibly irrKlang)
- Add level generation :D?
