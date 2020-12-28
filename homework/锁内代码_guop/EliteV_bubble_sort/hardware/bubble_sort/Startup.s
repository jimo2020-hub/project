

;----------------------- CODE --------------------------------------------------
                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA |.text|, CODE, READONLY   

				EXPORT __Vectors
__Vectors
				DCD   0x20003000
				DCD   0x00000009
				BL		Reset_Handler
				
Reset_Handler	PROC 
				EXPORT  Reset_Handler
                IMPORT  __use_two_region_memory

; Enter the C code -------------------------------------------------------------
				; IMPORT __rt_entry
				; BL __rt_entry
				IMPORT __main
				BL __main
				
				ENDP

                END

