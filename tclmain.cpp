#include "inst.hpp"
#include "host_system.hpp"
#include "simple_arch_state.hpp"
#include <tcl.h>

class RvFunCtxt
{
public:
	RvFunCtxt()
	{
		state_.setSys(&host_);
		state_.setMem(host_.getMem());
	}

	void addCommands(Tcl_Interp *interp)
	{
		Tcl_CreateObjCommand(interp, "rvfun::disasm", &disasm, NULL, NULL);
	}

private: // methods
	/// context free disassembly.
	/// Usage: rvfun::disasm <opcode>
	static
	int disasm(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
	{
		if (objc < 2)
		{
			Tcl_SetObjResult(interp, Tcl_NewStringObj("Usage: rvfun disasm <opcode>", -1));
			return TCL_ERROR;
		}

		// pull the integer value
		int opcode = 0;
		int retval = Tcl_GetIntFromObj(interp, objv[1], &opcode);
		if (retval != TCL_OK)
		{
			Tcl_SetObjResult(interp, Tcl_NewStringObj("Unable to get integer from argument.", -1));
			return TCL_ERROR;
		}

		// make an instruction
		rvfun::Inst *inst = nullptr;
		if ((opcode & 3) == 3)
		{
			inst = rvfun::decode32(opcode);
		}
		else
		{
			inst = rvfun::decode16(opcode);
		}

		if (!inst)
		{
			Tcl_SetObjResult(interp, Tcl_NewStringObj("(null inst)", -1));
			return TCL_OK;
		}

		const std::string val = inst->disasm();
		Tcl_SetObjResult(interp, Tcl_NewStringObj(val.c_str(), val.size()));
		delete inst;

		return TCL_OK;
	}

private: // data
	rvfun::HostSystem host_;
	rvfun::SimpleArchState state_;
};

extern "C"
int Rvfuntcl_Init(Tcl_Interp *interp)
{
	if (Tcl_InitStubs(interp, "8.6", 0) == NULL)
		return TCL_ERROR;

	Tcl_Namespace *name_space = Tcl_CreateNamespace(interp, "rvfun", NULL, NULL);
	// export all subcommands
	int retval = Tcl_Export(interp, name_space, "*", 0);
	if (retval) {
		return TCL_ERROR;
	}

	RvFunCtxt *ctxt = new RvFunCtxt;
	ctxt->addCommands(interp);
	Tcl_CreateEnsemble(interp, "rvfun", name_space, 0);
	Tcl_Import(interp, NULL, "rvfun::*", 0);

	return TCL_OK;
}

