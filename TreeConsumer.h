#pragma once

#include "TreeVisitor.h"
#include "Utils.h"

#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"

class TreeConsumer : public clang::ASTConsumer {
private:
    TreeVisitor *visitor;
    clang::SourceManager& smanager;

public:

    std::vector<std::string> stdheaders{"assert.h","complex.h","ctype.h","errno.h","fenv.h","float.h","inttypes.h","iso646.h","limits.h",
                                        "locale.h","math.h","setjmp.h","signal.h","stdalign.h","stdarg.h","stdatomic.h","stdbool.h","stddef.h",
                                        "stdint.h","stdio.h","stdlib.h","stdnoreturn.h","string.h","tgmath.h","threads.h","time.h","uchar.h",
                                        "wchar.h","wctype.h","aio.h","inet.h","assert.h","complex.h","cpio.h","ctype.h","dirent.h","dlfcn.h",
                                        "errno.h","fcntl.h","fenv.h","float.h","fmtmsg.h","fnmatch.h","ftw.h","glob.h","grp.h","iconv.h",
                                        "inttypes.h","iso646.h","langinfo.h","libgen.h","limits.h","locale.h","math.h","monetary.h","mqueue.h",
                                        "ndbm.h","if.h","netdb.h","in.h","tcp.h","nl_types.h","poll.h","pthread.h","pwd.h",
                                        "regex.h","sched.h","search.h","semaphore.h","setjmp.h","signal.h","spawn.h","stdarg.h","stdbool.h",
                                        "stddef.h","stdint.h","stdio.h","stdlib.h","string.h","strings.h","stropts.h","ipc.h","mman.h",
                                        "msg.h","resource.h","select.h","sem.h","shm.h","socket.h","stat.h",
                                        "statvfs.h","time.h","times.h","types.h","uio.h","un.h","utsname.h",
                                        "wait.h","syslog.h","tar.h","termios.h","tgmath.h","time.h","trace.h","ulimit.h","unistd.h",
                                        "utime.h","utmpx.h","wchar.h","wctype.h","wordexp.h",

                                        "siginfo.h", "sigset.h", "sigaction.h", "sigcontext.h", "sigthread.h", "ucontext.h",
                                        "pthreadtypes.h", "libio.h", "__stddef_max_align_t.h", "_G_config.h", "sys_errlist.h",
                                        "sysmacros.h", "xlocale.h", "socket_type.h", "sockaddr.h", "sigstack.h"
                                       };

    /* with folders
    std::vector<std::string> stdheaders{"assert.h","complex.h","ctype.h","errno.h","fenv.h","float.h","inttypes.h","iso646.h","limits.h",
                                        "locale.h","math.h","setjmp.h","signal.h","stdalign.h","stdarg.h","stdatomic.h","stdbool.h","stddef.h",
                                        "stdint.h","stdio.h","stdlib.h","stdnoreturn.h","string.h","tgmath.h","threads.h","time.h","uchar.h",
                                        "wchar.h","wctype.h","aio.h","arpa/inet.h","assert.h","complex.h","cpio.h","ctype.h","dirent.h","dlfcn.h",
                                        "errno.h","fcntl.h","fenv.h","float.h","fmtmsg.h","fnmatch.h","ftw.h","glob.h","grp.h","iconv.h",
                                        "inttypes.h","iso646.h","langinfo.h","libgen.h","limits.h","locale.h","math.h","monetary.h","mqueue.h",
                                        "ndbm.h","net/if.h","netdb.h","netinet/in.h","netinet/tcp.h","nl_types.h","poll.h","pthread.h","pwd.h",
                                        "regex.h","sched.h","search.h","semaphore.h","setjmp.h","signal.h","spawn.h","stdarg.h","stdbool.h",
                                        "stddef.h","stdint.h","stdio.h","stdlib.h","string.h","strings.h","stropts.h","sys/ipc.h","sys/mman.h",
                                        "sys/msg.h","sys/resource.h","sys/select.h","sys/sem.h","sys/shm.h","sys/socket.h","sys/stat.h",
                                        "sys/statvfs.h","sys/time.h","sys/times.h","sys/types.h","sys/uio.h","sys/un.h","sys/utsname.h",
                                        "sys/wait.h","syslog.h","tar.h","termios.h","tgmath.h","time.h","trace.h","ulimit.h","unistd.h",
                                        "utime.h","utmpx.h","wchar.h","wctype.h","wordexp.h"};
     */

    explicit TreeConsumer(clang::CompilerInstance *CI) : visitor(new TreeVisitor(CI)), smanager(CI->getASTContext().getSourceManager()) {}



    virtual bool HandleTopLevelDecl(clang::DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (clang::DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            clang::Decl *D = *i;
            std::string fpath = smanager.getFilename(D->getLocation()).str();
            //llvm::errs() << "DEBUG: " << fpath << "\n";
            if(std::find(stdheaders.begin(), stdheaders.end(), basename(fpath)) == stdheaders.end() && fpath != ""){
                visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
            }
        }
        return true;
    }

    // this replaces "HandleTopLevelDecl"
    // override this to call our ExampleVisitor on the entire source file
    /*virtual void HandleTranslationUnit(clang::ASTContext &Context) {
        //we can use ASTContext to get the TranslationUnitDecl, which is
        //a single Decl that collectively represents the entire source file
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }*/

};
