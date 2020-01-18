/*
This component passes alert messages to the template whenever 
a message is recieved from the alert service. Accomplishes this
by subscribing to the alert service's getMessage() method which 
returns an Observable. 
*/
import { Component, OnInit } from '@angular/core';

import { AlertService } from '../_services/index';

@Component({
    moduleId: module.id.toString(),
    selector: 'alert',
    templateUrl: 'alert.component.html'
})

export class AlertComponent {
    message: any;

    constructor(private alertService: AlertService) { }

    ngOnInit() {
        this.alertService.getMessage().subscribe(message => { this.message = message; });
    }
}