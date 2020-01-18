/*
Leaflet model which is used in conjunction with leaflet.component.ts
to generate the intial map. 
*/
export class LeafletModel {

	constructor(
		public latitude: number = 0,
		public longitude: number = 0,
		public zoom: number = 4,
		public zoomLevels: number[] = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 ]
	) { }

}