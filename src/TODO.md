Here are the parameters I would like to see either locked, hidden or removed in this light version.
- Sensor ICO Variation Correction 
- Sensor ICO Calibration capability 
- Transmit Power supply pin (should be locked to optimal setting)
- High Pass noise reduction Function (should be locked to optimal setting)
- Variable drift rate (it’s always disabled now anyway)
- CTSU Information buffer names shouldn’t be configurable (I don’t understand the usecase), but if we want them to be configurable then we should remove or update verbiage from Workbench6 to CTW or whatever they’re inherited from. 

-The SFR settings:
CTSUSSDIV
CTSUSNUM
CTSUICOG
CTSUDPA
CTSURICOA
CTSUSO

I would prefer the PCLKB divisor, and CLK freq be inherited but I remember hearing that wasn’t possible.
- We need a better CGC module which can provide the already calculated information to maintain single point of change.
