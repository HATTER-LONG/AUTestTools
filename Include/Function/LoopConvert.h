#include <clang/Frontend/FrontendActions.h>
#include <clang/Rewrite/Core/Rewriter.h>

namespace clang
{
    // For each source file provided to the tool, a new FrontendAction is created.
    class MyFrontendAction : public ASTFrontendAction
    {
    public:
        MyFrontendAction() = default;
        void EndSourceFileAction() override;

        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef file) override;

    private:
        Rewriter TheRewriter;
    };
} // namespace clang

int Function(int argc, const char** argv);