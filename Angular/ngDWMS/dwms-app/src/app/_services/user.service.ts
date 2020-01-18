/*
Contains the standard set of CRUD operations for managing users.
It acts as the interace between the Angular application and the 
backend API.
*/
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { User } from '../_models/index';

@Injectable()
export class UserService {
    constructor(private http: HttpClient) { }

    getAllUsers() {
        return this.http.get<User[]>('/api/users');
    }

    getById(id: number) {
        return this.http.get('/api/users/' + id);
    }

    addUser(user: User) {
        return this.http.post('/api/users', user);
    }

    update(user: User) {
        return this.http.put('/api/users/' + user.id, user);
    }

    deleteUserById(id: number) {
        return this.http.delete('/api/users/' + id);
    }
}

