namespace edu {
	namespace sharif {
		namespace twinner {
			namespace trace {
				class Expression {
			
				private:
					int stack;
					int lastConcreteValue;
			
				public:
					void toString();
				
					void unaryOperation(edu::sharif::twinner::trace::Operator op, edu::sharif::twinner::trace::Expression exp);
				
					void binaryOperation(edu::sharif::twinner::trace::Operator op, edu::sharif::twinner::trace::Expression exp);
				
					edu::sharif::twinner::trace::Expression clone();
				};
			}
		}
	}
}
