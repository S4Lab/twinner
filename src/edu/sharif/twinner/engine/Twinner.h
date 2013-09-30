namespace edu {
	namespace sharif {
		namespace twinner {
			namespace engine {
				class Twinner {
			

				public:
					void setPinLauncherPath();
				
					void setInputBinaryPath();
				
					void setTwinBinaryPath();
				
					void generateTwinBinary();
			
				private:
					void calculateSymbolsValuesForCoveringNextPath();
				
					void codeTracesIntoTwinBinary();
				};
			}
		}
	}
}
