/*-
 * Copyright 2017 Diomidis Spinellis, Thodoris Sotiropoulos
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef FUNCTIONSIGNATURE_H
#define FUNCTIONSIGNATURE_H


/** Information about the signature of a function. */
class FunctionSignature {
private:
	int nparams;
public:
	FunctionSignature() { reset(); }

	void reset() { nparams = 0; }

	void add_param() { nparams++; }

	int get_nparams() { return nparams; }

};
#endif /* FUNCTIONSIGNATURE_H */
