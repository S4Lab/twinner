namespace edu {
	namespace sharif {
		namespace twinner {
			namespace trace {
				class Trace {
			

				private:
					void getCurrentTraceSegment();
			
				public:
					edu::sharif::twinner::trace::Expression getSymbolicExpression(int address);
				
					void addPathConstraint(edu::sharif::twinner::trace::Constraint c);
				
					void setSymbolicExpression(int address, edu::sharif::twinner::trace::Expression exp);
				
					void syscallInvoked(edu::sharif::twinner::trace::Syscall s);
				
					void saveToFile();
				
					void loadFromFile();
				};
			}
		}
	}
}
