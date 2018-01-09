//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "X86Windows7Sp1Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace syscall {

const Syscall::SyscallInformation X86Windows7Sp1Syscall::SYSCALLS_TABLE[406] = {
  {0x0000, "NtAcceptConnectPort"},
  {0x0001, "NtAccessCheck"},
  {0x0002, "NtAccessCheckAndAuditAlarm"},
  {0x0003, "NtAccessCheckByType"},
  {0x0004, "NtAccessCheckByTypeAndAuditAlarm"},
  {0x0005, "NtAccessCheckByTypeResultList"},
  {0x0006, "NtAccessCheckByTypeResultListAndAuditAlarm"},
  {0x0007, "NtAccessCheckByTypeResultListAndAuditAlarmByHandle"},
  {0x0008, "NtAddAtom"},
  {0x0009, "NtAddBootEntry"},
  {0x000a, "NtAddDriverEntry"},
  {0x000b, "NtAdjustGroupsToken"},
  {0x000c, "NtAdjustPrivilegesToken"},
  {0x000d, "NtAlertResumeThread"},
  {0x000e, "NtAlertThread"},
  {0x000f, "NtAllocateLocallyUniqueId"},
  {0x0010, "NtAllocateReserveObject"},
  {0x0011, "NtAllocateUserPhysicalPages"},
  {0x0012, "NtAllocateUuids"},
  {0x0013, "NtAllocateVirtualMemory"},
  {0x0014, "NtAlpcAcceptConnectPort"},
  {0x0015, "NtAlpcCancelMessage"},
  {0x0016, "NtAlpcConnectPort"},
  {0x0017, "NtAlpcCreatePort"},
  {0x0018, "NtAlpcCreatePortSection"},
  {0x0019, "NtAlpcCreateResourceReserve"},
  {0x001a, "NtAlpcCreateSectionView"},
  {0x001b, "NtAlpcCreateSecurityContext"},
  {0x001c, "NtAlpcDeletePortSection"},
  {0x001d, "NtAlpcDeleteResourceReserve"},
  {0x001e, "NtAlpcDeleteSectionView"},
  {0x001f, "NtAlpcDeleteSecurityContext"},
  {0x0020, "NtAlpcDisconnectPort"},
  {0x0021, "NtAlpcImpersonateClientOfPort"},
  {0x0022, "NtAlpcOpenSenderProcess"},
  {0x0023, "NtAlpcOpenSenderThread"},
  {0x0024, "NtAlpcQueryInformation"},
  {0x0025, "NtAlpcQueryInformationMessage"},
  {0x0026, "NtAlpcRevokeSecurityContext"},
  {0x0027, "NtAlpcSendWaitReceivePort"},
  {0x0028, "NtAlpcSetInformation"},
  {0x0029, "NtApphelpCacheControl"},
  {0x002a, "NtAreMappedFilesTheSame"},
  {0x002b, "NtAssignProcessToJobObject"},
  {0x002c, "NtCallbackReturn"},
  {0x002d, "NtCancelIoFile"},
  {0x002e, "NtCancelIoFileEx"},
  {0x002f, "NtCancelSynchronousIoFile"},
  {0x0030, "NtCancelTimer"},
  {0x0031, "NtClearEvent"},
  {0x0032, "NtClose"},
  {0x0033, "NtCloseObjectAuditAlarm"},
  {0x0034, "NtCommitComplete"},
  {0x0035, "NtCommitEnlistment"},
  {0x0036, "NtCommitTransaction"},
  {0x0037, "NtCompactKeys"},
  {0x0038, "NtCompareTokens"},
  {0x0039, "NtCompleteConnectPort"},
  {0x003a, "NtCompressKey"},
  {0x003b, "NtConnectPort"},
  {0x003c, "NtContinue"},
  {0x003d, "NtCreateDebugObject"},
  {0x003e, "NtCreateDirectoryObject"},
  {0x003f, "NtCreateEnlistment"},
  {0x0040, "NtCreateEvent"},
  {0x0041, "NtCreateEventPair"},
  {0x0042, "NtCreateFile"},
  {0x0043, "NtCreateIoCompletion"},
  {0x0044, "NtCreateJobObject"},
  {0x0045, "NtCreateJobSet"},
  {0x0046, "NtCreateKey"},
  {0x0048, "NtCreateKeyTransacted"},
  {0x0047, "NtCreateKeyedEvent"},
  {0x0049, "NtCreateMailslotFile"},
  {0x004a, "NtCreateMutant"},
  {0x004b, "NtCreateNamedPipeFile"},
  {0x004c, "NtCreatePagingFile"},
  {0x004d, "NtCreatePort"},
  {0x004e, "NtCreatePrivateNamespace"},
  {0x004f, "NtCreateProcess"},
  {0x0050, "NtCreateProcessEx"},
  {0x0051, "NtCreateProfile"},
  {0x0052, "NtCreateProfileEx"},
  {0x0053, "NtCreateResourceManager"},
  {0x0054, "NtCreateSection"},
  {0x0055, "NtCreateSemaphore"},
  {0x0056, "NtCreateSymbolicLinkObject"},
  {0x0057, "NtCreateThread"},
  {0x0058, "NtCreateThreadEx"},
  {0x0059, "NtCreateTimer"},
  {0x005a, "NtCreateToken"},
  {0x005b, "NtCreateTransaction"},
  {0x005c, "NtCreateTransactionManager"},
  {0x005d, "NtCreateUserProcess"},
  {0x005e, "NtCreateWaitablePort"},
  {0x005f, "NtCreateWorkerFactory"},
  {0x0060, "NtDebugActiveProcess"},
  {0x0061, "NtDebugContinue"},
  {0x0062, "NtDelayExecution"},
  {0x0063, "NtDeleteAtom"},
  {0x0064, "NtDeleteBootEntry"},
  {0x0065, "NtDeleteDriverEntry"},
  {0x0066, "NtDeleteFile"},
  {0x0067, "NtDeleteKey"},
  {0x0068, "NtDeleteObjectAuditAlarm"},
  {0x0069, "NtDeletePrivateNamespace"},
  {0x006a, "NtDeleteValueKey"},
  {0x006b, "NtDeviceIoControlFile"},
  {0x006c, "NtDisableLastKnownGood"},
  {0x006d, "NtDisplayString"},
  {0x006e, "NtDrawText"},
  {0x006f, "NtDuplicateObject"},
  {0x0070, "NtDuplicateToken"},
  {0x0071, "NtEnableLastKnownGood"},
  {0x0072, "NtEnumerateBootEntries"},
  {0x0073, "NtEnumerateDriverEntries"},
  {0x0074, "NtEnumerateKey"},
  {0x0075, "NtEnumerateSystemEnvironmentValuesEx"},
  {0x0076, "NtEnumerateTransactionObject"},
  {0x0077, "NtEnumerateValueKey"},
  {0x0078, "NtExtendSection"},
  {0x0079, "NtFilterToken"},
  {0x007a, "NtFindAtom"},
  {0x007b, "NtFlushBuffersFile"},
  {0x007c, "NtFlushInstallUILanguage"},
  {0x007d, "NtFlushInstructionCache"},
  {0x007e, "NtFlushKey"},
  {0x007f, "NtFlushProcessWriteBuffers"},
  {0x0080, "NtFlushVirtualMemory"},
  {0x0081, "NtFlushWriteBuffer"},
  {0x0082, "NtFreeUserPhysicalPages"},
  {0x0083, "NtFreeVirtualMemory"},
  {0x0084, "NtFreezeRegistry"},
  {0x0085, "NtFreezeTransactions"},
  {0x0086, "NtFsControlFile"},
  {0x0087, "NtGetContextThread"},
  {0x0088, "NtGetCurrentProcessorNumber"},
  {0x0089, "NtGetDevicePowerState"},
  {0x008a, "NtGetMUIRegistryInfo"},
  {0x008b, "NtGetNextProcess"},
  {0x008c, "NtGetNextThread"},
  {0x008d, "NtGetNlsSectionPtr"},
  {0x008e, "NtGetNotificationResourceManager"},
  {0x008f, "NtGetPlugPlayEvent"},
  {0x0090, "NtGetWriteWatch"},
  {0x0091, "NtImpersonateAnonymousToken"},
  {0x0092, "NtImpersonateClientOfPort"},
  {0x0093, "NtImpersonateThread"},
  {0x0094, "NtInitializeNlsFiles"},
  {0x0095, "NtInitializeRegistry"},
  {0x0096, "NtInitiatePowerAction"},
  {0x0097, "NtIsProcessInJob"},
  {0x0098, "NtIsSystemResumeAutomatic"},
  {0x0099, "NtIsUILanguageComitted"},
  {0x009a, "NtListenPort"},
  {0x009b, "NtLoadDriver"},
  {0x009c, "NtLoadKey"},
  {0x009d, "NtLoadKey2"},
  {0x009e, "NtLoadKeyEx"},
  {0x009f, "NtLockFile"},
  {0x00a0, "NtLockProductActivationKeys"},
  {0x00a1, "NtLockRegistryKey"},
  {0x00a2, "NtLockVirtualMemory"},
  {0x00a3, "NtMakePermanentObject"},
  {0x00a4, "NtMakeTemporaryObject"},
  {0x00a5, "NtMapCMFModule"},
  {0x00a6, "NtMapUserPhysicalPages"},
  {0x00a7, "NtMapUserPhysicalPagesScatter"},
  {0x00a8, "NtMapViewOfSection"},
  {0x00a9, "NtModifyBootEntry"},
  {0x00aa, "NtModifyDriverEntry"},
  {0x00ab, "NtNotifyChangeDirectoryFile"},
  {0x00ac, "NtNotifyChangeKey"},
  {0x00ad, "NtNotifyChangeMultipleKeys"},
  {0x00ae, "NtNotifyChangeSession"},
  {0x00af, "NtOpenDirectoryObject"},
  {0x00b0, "NtOpenEnlistment"},
  {0x00b1, "NtOpenEvent"},
  {0x00b2, "NtOpenEventPair"},
  {0x00b3, "NtOpenFile"},
  {0x00b4, "NtOpenIoCompletion"},
  {0x00b5, "NtOpenJobObject"},
  {0x00b6, "NtOpenKey"},
  {0x00b7, "NtOpenKeyEx"},
  {0x00b9, "NtOpenKeyTransacted"},
  {0x00ba, "NtOpenKeyTransactedEx"},
  {0x00b8, "NtOpenKeyedEvent"},
  {0x00bb, "NtOpenMutant"},
  {0x00bc, "NtOpenObjectAuditAlarm"},
  {0x00bd, "NtOpenPrivateNamespace"},
  {0x00be, "NtOpenProcess"},
  {0x00bf, "NtOpenProcessToken"},
  {0x00c0, "NtOpenProcessTokenEx"},
  {0x00c1, "NtOpenResourceManager"},
  {0x00c2, "NtOpenSection"},
  {0x00c3, "NtOpenSemaphore"},
  {0x00c4, "NtOpenSession"},
  {0x00c5, "NtOpenSymbolicLinkObject"},
  {0x00c6, "NtOpenThread"},
  {0x00c7, "NtOpenThreadToken"},
  {0x00c8, "NtOpenThreadTokenEx"},
  {0x00c9, "NtOpenTimer"},
  {0x00ca, "NtOpenTransaction"},
  {0x00cb, "NtOpenTransactionManager"},
  {0x00cc, "NtPlugPlayControl"},
  {0x00cd, "NtPowerInformation"},
  {0x00d0, "NtPrePrepareComplete"},
  {0x00d1, "NtPrePrepareEnlistment"},
  {0x00ce, "NtPrepareComplete"},
  {0x00cf, "NtPrepareEnlistment"},
  {0x00d2, "NtPrivilegeCheck"},
  {0x00d4, "NtPrivilegeObjectAuditAlarm"},
  {0x00d3, "NtPrivilegedServiceAuditAlarm"},
  {0x00d5, "NtPropagationComplete"},
  {0x00d6, "NtPropagationFailed"},
  {0x00d7, "NtProtectVirtualMemory"},
  {0x00d8, "NtPulseEvent"},
  {0x00d9, "NtQueryAttributesFile"},
  {0x00da, "NtQueryBootEntryOrder"},
  {0x00db, "NtQueryBootOptions"},
  {0x00dc, "NtQueryDebugFilterState"},
  {0x00dd, "NtQueryDefaultLocale"},
  {0x00de, "NtQueryDefaultUILanguage"},
  {0x00df, "NtQueryDirectoryFile"},
  {0x00e0, "NtQueryDirectoryObject"},
  {0x00e1, "NtQueryDriverEntryOrder"},
  {0x00e2, "NtQueryEaFile"},
  {0x00e3, "NtQueryEvent"},
  {0x00e4, "NtQueryFullAttributesFile"},
  {0x00e5, "NtQueryInformationAtom"},
  {0x00e6, "NtQueryInformationEnlistment"},
  {0x00e7, "NtQueryInformationFile"},
  {0x00e8, "NtQueryInformationJobObject"},
  {0x00e9, "NtQueryInformationPort"},
  {0x00ea, "NtQueryInformationProcess"},
  {0x00eb, "NtQueryInformationResourceManager"},
  {0x00ec, "NtQueryInformationThread"},
  {0x00ed, "NtQueryInformationToken"},
  {0x00ee, "NtQueryInformationTransaction"},
  {0x00ef, "NtQueryInformationTransactionManager"},
  {0x00f0, "NtQueryInformationWorkerFactory"},
  {0x00f1, "NtQueryInstallUILanguage"},
  {0x00f2, "NtQueryIntervalProfile"},
  {0x00f3, "NtQueryIoCompletion"},
  {0x00f4, "NtQueryKey"},
  {0x00f5, "NtQueryLicenseValue"},
  {0x00f6, "NtQueryMultipleValueKey"},
  {0x00f7, "NtQueryMutant"},
  {0x00f8, "NtQueryObject"},
  {0x00f9, "NtQueryOpenSubKeys"},
  {0x00fa, "NtQueryOpenSubKeysEx"},
  {0x00fb, "NtQueryPerformanceCounter"},
  {0x00fc, "NtQueryPortInformationProcess"},
  {0x00fd, "NtQueryQuotaInformationFile"},
  {0x00fe, "NtQuerySection"},
  {0x00ff, "NtQuerySecurityAttributesToken"},
  {0x0100, "NtQuerySecurityObject"},
  {0x0101, "NtQuerySemaphore"},
  {0x0102, "NtQuerySymbolicLinkObject"},
  {0x0103, "NtQuerySystemEnvironmentValue"},
  {0x0104, "NtQuerySystemEnvironmentValueEx"},
  {0x0105, "NtQuerySystemInformation"},
  {0x0106, "NtQuerySystemInformationEx"},
  {0x0107, "NtQuerySystemTime"},
  {0x0108, "NtQueryTimer"},
  {0x0109, "NtQueryTimerResolution"},
  {0x010a, "NtQueryValueKey"},
  {0x010b, "NtQueryVirtualMemory"},
  {0x010c, "NtQueryVolumeInformationFile"},
  {0x010d, "NtQueueApcThread"},
  {0x010e, "NtQueueApcThreadEx"},
  {0x010f, "NtRaiseException"},
  {0x0110, "NtRaiseHardError"},
  {0x0111, "NtReadFile"},
  {0x0112, "NtReadFileScatter"},
  {0x0113, "NtReadOnlyEnlistment"},
  {0x0114, "NtReadRequestData"},
  {0x0115, "NtReadVirtualMemory"},
  {0x0116, "NtRecoverEnlistment"},
  {0x0117, "NtRecoverResourceManager"},
  {0x0118, "NtRecoverTransactionManager"},
  {0x0119, "NtRegisterProtocolAddressInformation"},
  {0x011a, "NtRegisterThreadTerminatePort"},
  {0x011b, "NtReleaseKeyedEvent"},
  {0x011c, "NtReleaseMutant"},
  {0x011d, "NtReleaseSemaphore"},
  {0x011e, "NtReleaseWorkerFactoryWorker"},
  {0x011f, "NtRemoveIoCompletion"},
  {0x0120, "NtRemoveIoCompletionEx"},
  {0x0121, "NtRemoveProcessDebug"},
  {0x0122, "NtRenameKey"},
  {0x0123, "NtRenameTransactionManager"},
  {0x0124, "NtReplaceKey"},
  {0x0125, "NtReplacePartitionUnit"},
  {0x0126, "NtReplyPort"},
  {0x0127, "NtReplyWaitReceivePort"},
  {0x0128, "NtReplyWaitReceivePortEx"},
  {0x0129, "NtReplyWaitReplyPort"},
  {0x012a, "NtRequestPort"},
  {0x012b, "NtRequestWaitReplyPort"},
  {0x012c, "NtResetEvent"},
  {0x012d, "NtResetWriteWatch"},
  {0x012e, "NtRestoreKey"},
  {0x012f, "NtResumeProcess"},
  {0x0130, "NtResumeThread"},
  {0x0131, "NtRollbackComplete"},
  {0x0132, "NtRollbackEnlistment"},
  {0x0133, "NtRollbackTransaction"},
  {0x0134, "NtRollforwardTransactionManager"},
  {0x0135, "NtSaveKey"},
  {0x0136, "NtSaveKeyEx"},
  {0x0137, "NtSaveMergedKeys"},
  {0x0138, "NtSecureConnectPort"},
  {0x0139, "NtSerializeBoot"},
  {0x013a, "NtSetBootEntryOrder"},
  {0x013b, "NtSetBootOptions"},
  {0x013c, "NtSetContextThread"},
  {0x013d, "NtSetDebugFilterState"},
  {0x013e, "NtSetDefaultHardErrorPort"},
  {0x013f, "NtSetDefaultLocale"},
  {0x0140, "NtSetDefaultUILanguage"},
  {0x0141, "NtSetDriverEntryOrder"},
  {0x0142, "NtSetEaFile"},
  {0x0143, "NtSetEvent"},
  {0x0144, "NtSetEventBoostPriority"},
  {0x0145, "NtSetHighEventPair"},
  {0x0146, "NtSetHighWaitLowEventPair"},
  {0x0147, "NtSetInformationDebugObject"},
  {0x0148, "NtSetInformationEnlistment"},
  {0x0149, "NtSetInformationFile"},
  {0x014a, "NtSetInformationJobObject"},
  {0x014b, "NtSetInformationKey"},
  {0x014c, "NtSetInformationObject"},
  {0x014d, "NtSetInformationProcess"},
  {0x014e, "NtSetInformationResourceManager"},
  {0x014f, "NtSetInformationThread"},
  {0x0150, "NtSetInformationToken"},
  {0x0151, "NtSetInformationTransaction"},
  {0x0152, "NtSetInformationTransactionManager"},
  {0x0153, "NtSetInformationWorkerFactory"},
  {0x0154, "NtSetIntervalProfile"},
  {0x0155, "NtSetIoCompletion"},
  {0x0156, "NtSetIoCompletionEx"},
  {0x0157, "NtSetLdtEntries"},
  {0x0158, "NtSetLowEventPair"},
  {0x0159, "NtSetLowWaitHighEventPair"},
  {0x015a, "NtSetQuotaInformationFile"},
  {0x015b, "NtSetSecurityObject"},
  {0x015c, "NtSetSystemEnvironmentValue"},
  {0x015d, "NtSetSystemEnvironmentValueEx"},
  {0x015e, "NtSetSystemInformation"},
  {0x015f, "NtSetSystemPowerState"},
  {0x0160, "NtSetSystemTime"},
  {0x0161, "NtSetThreadExecutionState"},
  {0x0162, "NtSetTimer"},
  {0x0163, "NtSetTimerEx"},
  {0x0164, "NtSetTimerResolution"},
  {0x0165, "NtSetUuidSeed"},
  {0x0166, "NtSetValueKey"},
  {0x0167, "NtSetVolumeInformationFile"},
  {0x0168, "NtShutdownSystem"},
  {0x0169, "NtShutdownWorkerFactory"},
  {0x016a, "NtSignalAndWaitForSingleObject"},
  {0x016b, "NtSinglePhaseReject"},
  {0x016c, "NtStartProfile"},
  {0x016d, "NtStopProfile"},
  {0x016e, "NtSuspendProcess"},
  {0x016f, "NtSuspendThread"},
  {0x0170, "NtSystemDebugControl"},
  {0x0171, "NtTerminateJobObject"},
  {0x0172, "NtTerminateProcess"},
  {0x0173, "NtTerminateThread"},
  {0x0174, "NtTestAlert"},
  {0x0175, "NtThawRegistry"},
  {0x0176, "NtThawTransactions"},
  {0x0177, "NtTraceControl"},
  {0x0178, "NtTraceEvent"},
  {0x0179, "NtTranslateFilePath"},
  {0x017a, "NtUmsThreadYield"},
  {0x017b, "NtUnloadDriver"},
  {0x017c, "NtUnloadKey"},
  {0x017d, "NtUnloadKey2"},
  {0x017e, "NtUnloadKeyEx"},
  {0x017f, "NtUnlockFile"},
  {0x0180, "NtUnlockVirtualMemory"},
  {0x0181, "NtUnmapViewOfSection"},
  {0x0182, "NtVdmControl"},
  {0x0183, "NtWaitForDebugEvent"},
  {0x0184, "NtWaitForKeyedEvent"},
  {0x0185, "NtWaitForMultipleObjects"},
  {0x0186, "NtWaitForMultipleObjects32"},
  {0x0187, "NtWaitForSingleObject"},
  {0x0188, "NtWaitForWorkViaWorkerFactory"},
  {0x0189, "NtWaitHighEventPair"},
  {0x018a, "NtWaitLowEventPair"},
  {0x018b, "NtWorkerFactoryWorkerReady"},
  {0x018c, "NtWriteFile"},
  {0x018d, "NtWriteFileGather"},
  {0x018e, "NtWriteRequestData"},
  {0x018f, "NtWriteVirtualMemory"},
  {0x0190, "NtYieldExecution"},
  {0x018d, "NtWriteFileGather"},
  {0x018e, "NtWriteRequestData"},
  {0x018f, "NtWriteVirtualMemory"},
  {0x0190, "NtYieldExecution"},
  {0, 0},
};

X86Windows7Sp1Syscall::X86Windows7Sp1Syscall (ADDRINT _syscallNumber,
    ADDRINT _arg0, ADDRINT _arg1, ADDRINT _arg2, ADDRINT _arg3,
    ADDRINT _arg4, ADDRINT _arg5) :
    Syscall (_syscallNumber, _arg0, _arg1, _arg2, _arg3, _arg4, _arg5) {
}

X86Windows7Sp1Syscall::~X86Windows7Sp1Syscall () {
}

bool X86Windows7Sp1Syscall::isProcessTerminatingSyscall () const {
  return getSyscallNumber () == 0x0172;
}

ADDRINT X86Windows7Sp1Syscall::getExitCodeArgument () const {
  return getArg1 ();
}

const Syscall::SyscallInformation *X86Windows7Sp1Syscall::begin () const {
  return X86Windows7Sp1Syscall::SYSCALLS_TABLE;
}

const Syscall::SyscallInformation *X86Windows7Sp1Syscall::end () const {
  return X86Windows7Sp1Syscall::SYSCALLS_TABLE + 405;
}

}
}
}
}
}
