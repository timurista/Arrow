//
//  States.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/17/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class States {
    
    // MARK: Properties
    var stateAbreviations: [String] = []
    var stateNames: [String] = []
    var state: [String: String] = ["AL": "Alabama", "AK": "Alaska", "AZ": "Arizona", "AR": "Arkansas", "CA": "California", "CO": "Colorado", "CT": "Connecticut", "DE": "Delaware", "FL": "Florida", "GA": "Georgia", "HI": "Hawaii", "ID": "Idaho", "IL": "Illinois", "IN": "Indiana", "IA": "Iowa", "KS": "Kansas", "KY": "Kentucky", "LA": "Louisiana", "ME": "Maine", "MD": "Maryland", "MA": "Massachusetts", "MI": "Michigan", "MN": "Minnesota", "MS": "Mississippi", "MO": "Missouri", "MT": "Montana", "NE": "Nebraska", "NV": "Nevada", "NH": "New Hampshire", "NJ": "New Jersey", "NM": "New Mexico", "NY": "New York", "NC": "North Carolina", "ND": "North Dakota", "OH": "Ohio", "OK": "Oklahoma", "OR": "Oregon", "PA": "Pennsylvania", "RI": "Rhode Island", "SC": "South Carolina", "SD": "South Dakota", "TN": "Tennessee", "TX": "Texas", "UT": "Utah", "VT": "Vermont", "VA": "Virginia", "WA": "Washington", "WV": "West Virginia", "WI": "Wisconsin", "WY": "Wyoming", "DC": "District of Columbia"]
    
    // MARK: Initializers
    init(){
        for (abrev, name) in state {
            stateAbreviations.append(abrev)
            stateNames.append(name)
        }
        stateAbreviations.sortInPlace()
        stateNames.sortInPlace()
    }
    
    // MARK: Functions
    
    func getAbreviation(stateName: String) -> String { // Gets state abreviation from state name
                                                    // !! Assumes state names are unique !!
        var abreviation: String = ""
        for (abrev, name) in state {
            if name == stateName {
                abreviation = abrev
            }
        }
        return abreviation
    }
}