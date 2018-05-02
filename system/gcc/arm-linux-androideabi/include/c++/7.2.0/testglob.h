/*
	
	This is a file for internal test utils.
	It is under proprietary license and is a property of n0n3m4 (Roman Lebedev).
	You explicitly cannot redistribute it in any form.
	You cannot copy it as well.
	Actually, you are not allowed to do anything with it.
	If a provision of this License is or becomes illegal, invalid or unenforceable in any jurisdiction, that shall not affect:
	1) the validity or enforceability in that jurisdiction of any other provision of this License; or
	2) the validity or enforceability in other jurisdictions of that or any other provision of this License.

*/

#if 0
package com.n0n3m4.bosses;

import java.util.Random;

import com.badlogic.gdx.graphics.g2d.Sprite;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer.ShapeType;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Polygon;
import com.badlogic.gdx.math.Vector2;
import com.n0n3m4.enemies.EnemyShip1;
import com.n0n3m4.enemies.EnemyShip2;
import com.n0n3m4.enemies.EnemyShip3;
import com.n0n3m4.enemies.guns.AroundGuns;
import com.n0n3m4.entities.Enemy;
import com.n0n3m4.entities.Gun;
import com.n0n3m4.game.PlayState;
import com.n0n3m4.player.Player;
import com.neet.main.Game;

public class BossSpawner extends Enemy {

	private final float BOSSSPAWNER_HEALTH=20000;
	private final float BOSSSPAWNER_SIZE=230;
	private Sprite gate;

	public BossSpawner(float x, float y, PlayState owner) {
		
		super(x,y,owner);
		
		hpmax = BOSSSPAWNER_HEALTH;
		hp = BOSSSPAWNER_HEALTH;
		
		width = BOSSSPAWNER_SIZE;
		height = BOSSSPAWNER_SIZE;
		
		phi=MathUtils.PI/2;
		
		maxRotationSpeed=0.f; //No rotation at all
		maxSpeed=0.f;		  //And no movement
		
		float[] verts=new float[16];
		
		verts[0] = -0.2f;
		verts[1] = 0.5f;
		verts[2] = 0.2f;
		verts[3] = 0.5f;
		
		
		verts[4] = 0.5f;
		verts[5] = 0.2f;
		verts[6] = 0.5f;
		verts[7] = -0.2f;
		
		verts[8] = 0.2f;
		verts[9] = -0.5f;
		verts[10]= -0.2f;
		verts[11]= -0.5f;
		
		verts[12]= -0.5f;
		verts[13]= -0.2f;
		verts[14]= -0.5f;
		verts[15]= 0.2f;
		
		for (int i=0;i<verts.length;i++)
			verts[i]/=1.2f;
		
		
		phys=new Polygon(verts);
		phys.setScale(height,width);
		
		sprite=new Sprite(Game.res.tex_boss3);
		sprite.setOriginCenter();
		sprite.setScale(width/sprite.getWidth(),height/sprite.getHeight());
		
		gate=new Sprite(Game.res.tex_boss3gate);
		gate.setOriginCenter();
		gate.setScale(width/gate.getWidth(),height/gate.getHeight());
		
		InitAIRadius(height,width);
		
		//for (int i=0;i<smallguns.length/2;i++)
		//{
		//	float dphi=i*2*MathUtils.PI/gunz.length-phioffset;
		/*smallguns[0]=new SpawnerGuns(owner.getBullets(), -0.5f, BOSSSPAWNER_LGUNOFF, this);
		smallguns[1]=new SpawnerGuns(owner.getBullets(), +0.0f, BOSSSPAWNER_LGUNOFF, this);
		smallguns[2]=new SpawnerGuns(owner.getBullets(), +0.5f, BOSSSPAWNER_LGUNOFF, this);
		
		smallguns[3]=new SpawnerGuns(owner.getBullets(), -0.5f, BOSSSPAWNER_RGUNOFF, this);
		smallguns[4]=new SpawnerGuns(owner.getBullets(), +0.0f, BOSSSPAWNER_RGUNOFF, this);
		smallguns[5]=new SpawnerGuns(owner.getBullets(), +0.5f, BOSSSPAWNER_RGUNOFF, this);
		
		biggun=new SpawnerRocketGun(owner.getBullets(), this, BOSSSPAWNER_MGUNOFF);*/
		//}
	}
	
	/*private float roundtopi(float delta)
	{
		while (delta>Math.PI) delta-=2*Math.PI;
		while (delta<-Math.PI) delta+=2*Math.PI;
		return delta;
	}
	
	public void aimplayer(float dt)
	{
		Player res=tgt;
		if (res!=null)
		{
			float angl=AngleTo(res);
			if (!Float.isNaN(angl)&&!Float.isInfinite(angl))
			{
				float dphi=roundtopi(angl-phi);
				if (Math.abs(dphi)>Math.abs(maxRotationSpeed*dt))
					w=maxRotationSpeed*Math.signum(dphi);
				else
					w=dphi/dt;
			}
		}
	}*/
	
	
	float hplimit=0;
	final float REGEN_PERSTAGE=2000;
	final float REGEN_SPEED=400;
	final float REGEN_TIME=REGEN_PERSTAGE/REGEN_SPEED;
	float curregentime=0.f;
	int curen=0;
	int curens=0;
	
	float[] stages=new float[]{BOSSSPAWNER_HEALTH-REGEN_PERSTAGE*1.5f,BOSSSPAWNER_HEALTH-REGEN_PERSTAGE*3.f,BOSSSPAWNER_HEALTH-REGEN_PERSTAGE*5.5f};
	int curstage=0;
	
	public void checkStageReached()
	{
		if (curstage>=stages.length)
			return;
		if (hp<stages[curstage])
		{
			isRegen=true;
			hplimit=hp+REGEN_PERSTAGE;
			curens=10;
			curen=0;
			curstage++;
			curregentime=0.f;
		}
	}
	
	public void spawnEnemy()
	{
		try{
		Class<?>[] clazzes=new Class<?>[]{EnemyShip1.class,EnemyShip2.class,EnemyShip3.class};
		int idx=new Random().nextInt(clazzes.length);
		Enemy e=(Enemy)clazzes[idx].getConstructors()[0].newInstance(new Object[]{x,y,owner});
		getOwner().SpawnEnemy(e);
		}
		catch (Exception e){e.printStackTrace();};
	}
	
	boolean isRegen=false;
	public void processRegen(float dt)
	{
		if (!isRegen) return;
		
		hp+=REGEN_SPEED*dt;
		curregentime+=dt;
		
		if (curen<curens)
		if (curregentime>=REGEN_TIME-REGEN_TIME*(curens-curen)/curens)
		{
			curen++;
			spawnEnemy();
		}
		
		if (curregentime>=REGEN_TIME*2)
		{
			isRegen=false;//To workaround when dps == regen (impossible but anyway)
		}
		
		if (hp>=hplimit)
		{
			hp=hplimit;
			isRegen=false;
		}
	}
	
	@Override
	public void NotifyAvoidCollision(Enemy o) {
		// Nothing
	}
	
	public void update(float dt) {
		//Don't notify about collisions
		
		x += vx * dt;
		y += vy * dt;
		phi += w * dt;
		
		if (phys!=null)
		{
			phys.setRotation(phi*MathUtils.radDeg);
			phys.setPosition(x, y);				//SetPosition?
		}
		
		checkStageReached();
		processRegen(dt);
	}

	@Override
	public void draw(SpriteBatch sb) {
		
		sprite.setCenter(x, y);
		sprite.setRotation(MathUtils.radDeg*phi-90);
		sprite.draw(sb);
		
		if (isRegen)
		{
			//Draw the gate
			gate.setCenter(x, y);
			gate.setRotation(MathUtils.radDeg*phi-90);
			float coeff;//[0;1]
			coeff=Math.min(1.f,(0.5f-Math.abs(Math.min(curregentime/REGEN_TIME,1.f)-0.5f))*20);
			float gatewidth=width*coeff;
			gate.setScale(gatewidth/gate.getWidth(),height/gate.getHeight());
			gate.draw(sb);
		}
		
	}

	@Override
	public void drawWireframe(ShapeRenderer sr) {
		sr.setColor(1, 0, 0, 1);
		sr.begin(ShapeType.Line);
		float[] verts=phys.getTransformedVertices();
		for(int i = 0, j = verts.length/2 - 1;
			i < verts.length/2;
			j = i++) {
			
			sr.line(verts[i*2], verts[i*2+1], verts[j*2], verts[j*2+1]);
			
		}
		sr.end();
	}

	@Override
	public void kill() {
		super.kill();
		getOwner().socialManager.notifyThirdBoss();
	}
	
	public final int BOSSSPAWNER_PROFIT=3000;
	
	@Override
	public int getProfit() {
		return BOSSSPAWNER_PROFIT;
	}

	@Override
	public float getCollisionDamage() {
		return hp;
	}

}
#endif
