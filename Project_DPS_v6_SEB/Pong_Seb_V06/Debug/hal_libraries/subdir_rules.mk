################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
hal_libraries/hal_UCS.obj: ../hal_libraries/hal_UCS.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04/hal_libraries" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power=all --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal_libraries/hal_UCS.d_raw" --obj_directory="hal_libraries" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_libraries/hal_lcd.obj: ../hal_libraries/hal_lcd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04/hal_libraries" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power=all --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal_libraries/hal_lcd.d_raw" --obj_directory="hal_libraries" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_libraries/hal_lcd_fonts.obj: ../hal_libraries/hal_lcd_fonts.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04/hal_libraries" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power=all --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal_libraries/hal_lcd_fonts.d_raw" --obj_directory="hal_libraries" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_libraries/hal_pmm.obj: ../hal_libraries/hal_pmm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04/hal_libraries" --include_path="C:/Users/stuar/workspace_v7/Pong_Seb_V04" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power=all --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="hal_libraries/hal_pmm.d_raw" --obj_directory="hal_libraries" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


