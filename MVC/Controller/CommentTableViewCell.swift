//
//  CommentTableViewCell.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class CommentTableViewCell: UITableViewCell {

    // MARK: Properties
    var commentToDisplay: Comment? {
        didSet {
            updateUI()
        }
    }
    
    @IBOutlet weak var profilePicture: UIImageView!
    @IBOutlet weak var userNameLabel: UILabel!
    @IBOutlet weak var dateLabel: UILabel!
    @IBOutlet weak var bodyText: UITextView!
    
    // MARK: Functions
    func updateUI() {
        
        // Round profile picture edges
        profilePicture.layer.cornerRadius = 6.4
        profilePicture.clipsToBounds = true
        
        // Reset information
        userNameLabel.text = nil
        dateLabel.text = nil
        bodyText.text = nil
        profilePicture.image = nil
        
        // Add new information
        if let newComment = self.commentToDisplay {
            userNameLabel.text = newComment.user.getName()
            dateLabel.text = newComment.getDate()
            bodyText.text = newComment.text
            profilePicture.image = newComment.user.getProfilePicture()
        }
    }
}
