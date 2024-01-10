List<word> cloudFields;
List<scalar> levelValues;
if (args.optionFound("usage"))
{
    Info << "Usage: getPenetration -cloudNames '(dieselCloud ammoniaCloud)' -levelValues '(0.75 0.9)'" << endl;
    Info << "1) The cloudNames are the registered names of the clouds in the case. " << endl;
    Info << "2) The levelValues are the mass thresholds at which the specified percentage of the mass have been reached." << endl;
    return 0;
}
if (args.optionFound("cloudFields"))
{
    args.optionLookup("cloudFields")() >> cloudFields;
}
if (args.optionFound("levelValues"))
{
    args.optionLookup("levelValues")() >> levelValues;
}

Info << "cloudFields:" << cloudFields << endl;
Info << "levelValues:" << levelValues << endl;

