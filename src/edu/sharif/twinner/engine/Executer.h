namespace edu {
	namespace sharif {
		namespace twinner {
			namespace engine {
				class Executer {
			

				public:
					Executer(int pinLauncher, int inputBinary);
				
					void setSymbolsValues();
				
					edu::sharif::twinner::trace::Trace executeSingleTrace();
				};
			}
		}
	}
}
