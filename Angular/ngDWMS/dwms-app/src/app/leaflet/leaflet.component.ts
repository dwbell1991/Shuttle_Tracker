/*
This component is the basic control of the Leaflet map. 
It uses the Leaflet model to specify instantiation constraints. 
Once it is instantiated changing options will do nothing. 
*/
import { Component } from '@angular/core';

import { latLng, LatLng, tileLayer } from 'leaflet';

import { LeafletModel } from './leaflet.model';

@Component({
	selector: 'leaflet',
	templateUrl: './leaflet.component.html'
})
export class LeafletComponent {

	/*
	 * This is a specification of the leaflet options
	 * The reason to duplicate this object is so we can easily render it to the template
	 */
	optionsSpec: {
		layers: any[],
		zoom: number,
		center: number[]
	} = {
		layers: [
			{
				url: 'http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
				maxZoom: 18,
				attribution: 'Open Street Map'
			}
		],
		zoom: 15,
		center: [ 41.247106, -96.016764 ]
	};

	// Fields for managing the form inputs and binding to leaflet zoom/center
	model = new LeafletModel(
		this.optionsSpec.center[0],
		this.optionsSpec.center[1],
		this.optionsSpec.zoom
	);
	zoom: number;
	center: LatLng;

	/*
	 * This are the leaflet map options that we're going to use for input binding
	 */

	options = {
		layers: this.optionsSpec.layers.map((l) => {
			return tileLayer(l.url, { maxZoom: l.maxZoom, attribution: l.attribution });
		}),
		zoom: this.optionsSpec.zoom,
		center: latLng(this.optionsSpec.center[0], this.optionsSpec.center[1])
	};

	onApply() {
		this.zoom = this.model.zoom;
		this.center = latLng(this.model.latitude, this.model.longitude);

		return false;
	}

}