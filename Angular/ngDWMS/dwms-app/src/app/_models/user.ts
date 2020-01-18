/* 
Model used to define properties of a user 
*/
export class User {
	id: number;
    username: string;
    password: string;
    firstName: string;
    lastName: string;

    //Specify values values on instantiation
    constructor(values: Object = {}) {
    	Object.assign(this, values);
	}
}
