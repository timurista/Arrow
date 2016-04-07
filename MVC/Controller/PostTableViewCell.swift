//
//  PostTableViewCell.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/5/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class PostTableViewCell: UITableViewCell {

    // MARK: Properties
    var postToDisplay: Post? {
        didSet {
            updateUI()
        }
    }
    private let defaults = NSUserDefaults.standardUserDefaults()
    private let userDefaultsKey = "userID"
    
    @IBOutlet weak var userNameLabel: UILabel!
    @IBOutlet weak var dateLabel: UILabel!
    @IBOutlet weak var bodyLabel: UILabel!
    @IBOutlet weak var likeImage: UIImageView!
    @IBOutlet weak var likeButton: UIButton!
    @IBOutlet weak var numberOfLikesLabel: UILabel!
    @IBOutlet weak var numberOfCommentsLabel: UILabel!
    @IBOutlet weak var moreImage: UIImageView!
    @IBOutlet weak var moreButton: UIButton!
    @IBOutlet weak var commentButton: UIButton!
    @IBOutlet weak var profilePicture: UIImageView!
    
    // MARK: Functions
    func updateUI() {
        
        profilePicture.layer.cornerRadius = 8
        profilePicture.clipsToBounds = true
        
        // Reset information
        userNameLabel.text = nil
        dateLabel.text = nil
        bodyLabel.text = nil
        numberOfLikesLabel.text = nil
        numberOfCommentsLabel.text = nil
        likeImage.image = UIImage(named: "Like")
        moreImage.hidden = true
        moreButton.hidden = true
        
        // Add new information
        if let newPost = self.postToDisplay {
            userNameLabel.text = newPost.user.getName()
            dateLabel.text = newPost.getDate()
            bodyLabel.text = newPost.text
            numberOfCommentsLabel.text = "\(newPost.numberOfComments)"
            numberOfLikesLabel.text = "\(newPost.numberOfLikes)"
            if newPost.liked {
                likeImage.image = UIImage(named: "Like-Red")
            }
            if newPost.user.userID == defaults.stringForKey(userDefaultsKey) {
                moreImage.hidden = false
                moreButton.hidden = false
            }
        }
    }
}
