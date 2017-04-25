#ifndef _MEDIAIO_PLUGIN_SEEXPR_IMAGE_SYNTH_EXPR_HPP_
#define _MEDIAIO_PLUGIN_SEEXPR_IMAGE_SYNTH_EXPR_HPP_

#include <SeExpression.h>

class ImageSynthExpr : public SeExpression
{
public:
    //! Constructor that takes the expression to parse
    ImageSynthExpr(const std::string& expr)
        : SeExpression(expr)
    {
    }

    //! Simple variable that just returns its internal value
    struct Var : public SeExprScalarVarRef
    {
        Var(const double val)
            : val(val)
        {
        }

        Var() {}

        double val; // independent variable
        void eval(const SeExprVarNode* /*node*/, SeVec3d& result) { result[0] = val; }
    };

    //! variable map
    mutable std::map<std::string, Var> vars;

    //! resolve function that only supports one external variable 'x'
    SeExprVarRef* resolveVar(const std::string& name) const
    {
        std::map<std::string, Var>::iterator i = vars.find(name);

        if(i != vars.end())
            return &i->second;

        return 0;
    }
};

#endif
