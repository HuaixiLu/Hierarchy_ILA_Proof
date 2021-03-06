#include <emesh_axi_master.h>
#include <emesh_axi_slave.h>
#include <emesh_axi_top.h>
#include <ilang/vtarget-out/vtarget_gen.h>

/// the function to generate configuration
VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration();
VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv);

/// Build the model
int main(int argc, char **argv) {
  // extract the configurations
  std::vector<std::string> design_files = {
    "emaxi.v",
    "emesh2packet.v",
    "em_se.v",
    "esaxi.v",
    "oh_dsync.v",
    "oh_fifo_sync.v",
    "oh_memory_dp.v",
    "oh_memory_ram.v",
    "packet2emesh.v"
  };

  auto vtg_cfg = SetConfiguration();
  //auto vtg_cfg = HandleArguments(argc, argv);

  // build the model
  EmeshAxiMasterBridge emaxi;
  EmeshAxiSlaveBridge esaxi;
  EmeshAxiTop etop;

  
  std::string verilog_file_name = "esaxi_w.v";
  std::ofstream fw_verilog(verilog_file_name);
  esaxi.wmodel.ExportToVerilog(fw_verilog);
  fw_verilog.close();

  verilog_file_name = "esaxi_r.v";
  fw_verilog.open(verilog_file_name);
  esaxi.rmodel.ExportToVerilog(fw_verilog);
  fw_verilog.close();

  verilog_file_name = "emaxi_w.v";
  fw_verilog.open(verilog_file_name);
  emaxi.wmodel.ExportToVerilog(fw_verilog);
  fw_verilog.close();

  verilog_file_name = "emaxi_r.v";
  fw_verilog.open(verilog_file_name);
  emaxi.rmodel.ExportToVerilog(fw_verilog);
  fw_verilog.close();


  return 0;
}


VerilogVerificationTargetGenerator::vtg_config_t HandleArguments(int argc, char **argv) {
  // the solver, the cosa environment
  // you can use a commandline parser if desired, but since it is not the main focus of
  // this demo, we skip it

  // set ilang option, operators like '<' will refer to unsigned arithmetics
  SetUnsignedComparison(true); 
  
  VerilogVerificationTargetGenerator::vtg_config_t ret;

  for(unsigned p = 1; p<argc; p++) {
    std::string arg = argv[p];
    auto split = arg.find("=");
    auto argName = arg.substr(0,split);
    auto param   = arg.substr(split+1);

    if(argName == "Solver")
      ret.CosaSolver = param;
    else if(argName == "Env")
      ret.CosaPyEnvironment = param;
    else if(argName == "Cosa")
      ret.CosaPath = param;
    // else unknown
    else {
      std::cerr<<"Unknown argument:" << argName << std::endl;
      std::cerr<<"Expecting Solver/Env/Cosa=???" << std::endl;
    }
  }

  ret.CosaGenTraceVcd = true;

  /// other configurations
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::NEW;
  ret.CosaGenJgTesterScript = true;
  //ret.CosaOtherSolverOptions = "--blackbox-array";
  //ret.ForceInstCheckReset = true;

  return ret;
}

VerilogVerificationTargetGenerator::vtg_config_t SetConfiguration() {

  // set ilang option, operators like '<' will refer to unsigned arithmetics
  SetUnsignedComparison(true); 
  
  VerilogVerificationTargetGenerator::vtg_config_t ret;
  ret.CosaSolver = "z3";
  ret.CosaPyEnvironment = "/ibuild/ilang-env/bin/activate";
  ret.CosaPath = "/ibuild/CoSA";
  ret.CosaGenTraceVcd = true;

  /// other configurations
  ret.PortDeclStyle = VlgVerifTgtGenBase::vtg_config_t::NEW;
  ret.CosaGenJgTesterScript = true;
  //ret.CosaOtherSolverOptions = "--blackbox-array";
  //ret.ForceInstCheckReset = true;

  return ret;
}