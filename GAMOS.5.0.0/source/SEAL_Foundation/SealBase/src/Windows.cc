//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/sysapi/Windows.h"
#ifdef _WIN32

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

// Implementation of auto-loaded code.

seal::AutoLoadLib WinStubNTDLL ("ntdll");
seal::AutoLoadLib WinStubKernel32 ("kernel32");
seal::AutoLoadLib WinStubPowrprof ("powrprof");

seal::AutoLoad<NTSTATUS (HANDLE hProcess, PROCESSINFOCLASS pic, PVOID pi, ULONG len, PULONG plen)>
    MyNtQueryInformationProcess (WinStubNTDLL, "NtQueryInformationProcess"); // FIXME: NTAPI?

seal::AutoLoad<NTSTATUS (POWER_INFORMATION_LEVEL level, PVOID pin, ULONG nin, PVOID pout, ULONG nout)>
    MyCallNtPowerInformation (WinStubPowrprof, "CallNtPowerInformation"); // FIXME: WINAPI?

seal::AutoLoad<BOOL (COMPUTER_NAME_FORMAT fmt, LPWSTR name, LPDWORD size)>
    MyGetComputerNameExW (WinStubKernel32, "GetComputerNameExW"); // FIXME: WINAPI?

seal::AutoLoad<BOOL (COMPUTER_NAME_FORMAT fmt, LPSTR name, LPDWORD size)>
    MyGetComputerNameExA (WinStubKernel32, "GetComputerNameExA"); // FIXME: WINAPI?

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#endif // _WIN32
