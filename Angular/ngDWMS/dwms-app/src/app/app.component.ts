/*
The app component is the root component of the application, it
defines the root tag of the app. 
*/
import { Component } from '@angular/core';

import '../assets/app.css';

@Component({
  moduleId:  module.id,		//Relative path for the templateUrl
  selector: 'app-root',
  templateUrl: './app.component.html',
})

export class AppComponent { }
