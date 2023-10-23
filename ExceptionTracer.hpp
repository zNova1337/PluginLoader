#pragma once

namespace ExceptionTracer
{
    std::string GetExceptionRecordInfo(_EXCEPTION_POINTERS* ep) {
        return formatS("Exception Record Info:\nException Address: %IX | Exception Code: %IX | Exception Flags: %IX | Exception Parameters: %d", ep->ExceptionRecord->ExceptionAddress,
            ep->ExceptionRecord->ExceptionCode, ep->ExceptionRecord->ExceptionFlags, ep->ExceptionRecord->NumberParameters);
    }

    std::string GetExceptionRegisters(_EXCEPTION_POINTERS* ep)
    {
        return formatS("Exception Context Info:\nEip/Rip: %IX\nEax/Rax: %IX\nEbp: %IX\nEbx: %IX\nEcx: %IX\nEdi: %IX\nEdx:%IX\nEFlags: %IX\nEsi: %IX\nEsp: %IX",
#ifdef _WIN64
            ep->ContextRecord->Rip,
#else
            ep->ContextRecord->Eip,
#endif

#ifdef _WIN64
            ep->ContextRecord->Rax,
            ep->ContextRecord->Rbp,
            ep->ContextRecord->Rbx,
            ep->ContextRecord->Rcx,
            ep->ContextRecord->Rdi,
            ep->ContextRecord->Rdx,
            ep->ContextRecord->EFlags,
            ep->ContextRecord->Rsi,
            ep->ContextRecord->Rsp);
#else
            ep->ContextRecord->Eax,
            ep->ContextRecord->Ebp,
            ep->ContextRecord->Ebx,
            ep->ContextRecord->Ecx,
            ep->ContextRecord->Edi,
            ep->ContextRecord->Edx,
            ep->ContextRecord->EFlags,
            ep->ContextRecord->Esi,
            ep->ContextRecord->Esp);
#endif
    }

    //void RaiseException() {//Testing
    //    void* t = nullptr;
    //    *reinterpret_cast<uintptr_t*>(t) = -1;//EXCEPTION_ACCESS_VIOLATION
    //}
}


int ExceptionFilter(DWORD code, struct _EXCEPTION_POINTERS* ep)
{
    if (code == EXCEPTION_ACCESS_VIOLATION)
    {
        std::cout << "[!!!]Exception ACCESS Violation" << std::endl;
        std::cout << "Details:\n" << ExceptionTracer::GetExceptionRecordInfo(ep) << "\n\n" << ExceptionTracer::GetExceptionRegisters(ep) << std::endl;
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        std::cout << "[!!!]Unknown exception" << std::endl;
        std::cout << "Exception Details:\n" << ExceptionTracer::GetExceptionRecordInfo(ep) << "\n\n" << ExceptionTracer::GetExceptionRegisters(ep) << std::endl;
        return EXCEPTION_EXECUTE_HANDLER;
    };
}