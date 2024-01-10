/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2022 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    reactingFoam

Description
    Transient solver for turbulent flow of compressible reacting fluids with
    optional mesh motion and mesh topology changes.

    Uses the flexible PIMPLE (PISO-SIMPLE) solution for time-resolved and
    pseudo-transient simulations.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "argList.H"
#include "fluidReactionThermo.H"
#include "combustionModel.H"
#include "compressibleMomentumTransportModels.H"
#include "fluidReactionThermophysicalTransportModel.H"
#include "multivariateScheme.H"
#include "pimpleControl.H"
#include "pressureReference.H"
#include "CorrectPhi.H"
#include "fvModels.H"
#include "fvConstraints.H"
#include "localEulerDdtScheme.H"
#include "fvcSmooth.H"
#include "sprayCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "postProcess.H"
    #include "configCustomizedOptions.H"
    #include "setRootCaseLists.H"
    #include "treatCustomizedOptions.H"
    #include "createTime.H"
    #include "createMesh.H"
    //#include "createDyMControls.H"
    //#include "initContinuityErrs.H"
    #include "createFields.H"
    //#include "createFieldRefs.H"
    //#include "createRhoUfIfPresent.H"


    turbulence->validate();
    
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    if(!isDir(runTime.path()/"postProcessing"))
    {
        mkDir(runTime.path()/"postProcessing");
    }
	OFstream outputFile(runTime.path()/"postProcessing"/"sprayPenetration");

    outputFile << "#Time" << tab;
    forAll(cloudFields, cloudField)
    {
        forAll(levelValues, levelValue)
        {
            outputFile << cloudFields[cloudField] << "@"
                << levelValues[levelValue] << tab;
        }
    }
    outputFile << endl;


    Info<< "\nStarting time loop\n" << endl;
    instantList timeDirs = timeSelector::select0(runTime, args);
    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;

        // output time value
        outputFile << runTime.time().value() << tab;

        #include "createMesh.H"
        //#include "createDyMControls.H"
        //#include "initContinuityErrs.H"
        #include "createFields.H"
        //#include "createFieldRefs.H"
        //#include "createRhoUfIfPresent.H"
        rho = thermo.rho();

        // check the existence of all preset cloud names
        forAll(cloudFields, cloudField)
        {
            // check the existence of the cloud field
            if (!mesh.objectRegistry::foundObject<cloud>(cloudFields[cloudField]))
            {
                FatalErrorInFunction
                << "Could not find cloud " << cloudFields[cloudField] << tab
                << "at time = " << runTime.time().value() << tab
                << exit(FatalError);
            }
            else
            {
                sprayCloud& spray = dynamic_cast<sprayCloud&>
                (mesh.objectRegistry::lookupObjectRef<cloud>(cloudFields[cloudField]));
                forAll(levelValues, levelValue)
                {
                    outputFile << spray.penetration(levelValues[levelValue]) << tab;
                }
            }
        }
        outputFile << endl;

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
