# getPenetration
Calculate penetration length given cloud names and penetration levels in **OpenFOAM-10**. The cloud type should be sprayCloud.

# Usage
run the following command to view the usage guidelines 
```
getPenetration -usage
``` 

An example:

```bash
getPenetration -cloudFields '(dieselCloud ammoniaCloud)' -levelValues '(0.75 0.9)'
```
