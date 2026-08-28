

- when first initialized, object should be in light, full color
- measure the amount of time it's in the light "white background" = time to "charge" the object with light
- when light is turned off (black background), the object should have a fresnel effect for subsurface scattering 
    and the overall color/lighting should decay slowly over time
- when light is turned on again, should be full color again (temporary: should have a translucent appearance, same as beginning)
- mechanism: color should decay slowly over time, some kind of quadratic interpolation? 
1. add charge time value to the saturation/value of the color of the original object
2. when light is turned off, saturation/value of color decreases quadratically as time goes on until it reaches total black 


!!!! convert to hsv, add time values to saturation and value, convert back to rgb? !!!!!

amount of time charged should decrease the multiplier on the time so that more time == lerp slower

logarithmic interpolation?
decay time depends on time under light + "intensity" of color atm

TODO:
- HDR + bloom
- model loading/parser
- smiski model
- dynamic time duration of decay
- change the decay function?
- add a floor
- make the smiski its own light source

