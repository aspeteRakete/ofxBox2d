/*
 *  ofxBox2dRect.cpp
 *  ofxBox2dExample
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dRect.h"



//------------------------------------------------

ofxBox2dRect::ofxBox2dRect() {
}

//------------------------------------------------
void ofxBox2dRect::setup(b2World * b2dworld, ofRectangle rec) {
	setup(b2dworld, rec.x, rec.y, rec.width, rec.height);
}

//------------------------------------------------
void ofxBox2dRect::setup(b2World * b2dworld, float x, float y, float w, float h) {
	
	if(b2dworld == NULL) {
		ofLog(OF_LOG_NOTICE, "- must have a valid world -");
		return;
	}
	
	if (ofGetRectMode() == OF_RECTMODE_CORNER) {
		//w/=2; h/=2; // ?? this is wrong, a size doesn't change with a rectmode...
		x += w/2.; y += h/2.;
	}
	
	_width	= w;
	_height	= h;
	
	b2PolygonShape shape;
  // set as box takes half width 
  // http://www.box2d.org/manual.html 2.2 Creating a Ground Box
	shape.SetAsBox(w/2./OFX_BOX2D_SCALE, h/2./OFX_BOX2D_SCALE);
	
	fixture.shape		= &shape;
	fixture.density		= density;
	fixture.friction	= friction;
	fixture.restitution = bounce;
	
	b2BodyDef bodyDef;
	if(density == 0.f)
		bodyDef.type	= b2_staticBody;
	else
		bodyDef.type	= b2_dynamicBody;
	bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);	
  bodyDef.fixedRotation = fixedRotation;
  bodyDef.angularDamping = angularDamping;
	
	
	body = b2dworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);

  dead = false;
  //alive = true;
	
}

//------------------------------------------------
float ofxBox2dRect::getWidth() {
	return _width;
}

//------------------------------------------------
float ofxBox2dRect::getHeight() {
	return _height;
}

//------------------------------------------------
void ofxBox2dRect::addRepulsionForce(float x, float y, float amt) {
	addRepulsionForce(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dRect::addRepulsionForce(ofVec2f pt, float amt) {
	if(body != NULL) {
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			
			if(poly) {
				b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
				for (int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
					b2Vec2 D = P - qt; 
					b2Vec2 F = amt * D;
					body->ApplyForce(-F, P);
				}                        
			}
		}
	}
}


//------------------------------------------------
// In ofxBox2dRect.h:
// We compute the force for all four (transformed) corners of the rect.
// This keeps the rect's orientation correct!
void ofxBox2dRect::addAttractionPoint (float x, float y, float amt) {
	addAttractionPoint(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dRect::addAttractionPoint (ofVec2f pt, float amt) {
	
	if(body != NULL) {
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			
			if(poly) {
				b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
				for (int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
					b2Vec2 D = P - qt; 
					b2Vec2 F = amt * D;
					body->ApplyForce(F, P);
				}                        
			}
		}
	}
}

//------------------------------------------------
void ofxBox2dRect::draw() {
	
	if(body == NULL) {
		return;	
	}
	
	const b2Transform& xf = body->GetTransform();
	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
		
		if(poly) {
			ofBeginShape();
			for(int i=0; i<poly->m_vertexCount; i++) {
				b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
				ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
			}
			ofEndShape(true);
		}
	}
	
	
}
















