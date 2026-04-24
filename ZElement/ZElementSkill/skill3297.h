#ifndef __CPPGEN_GNET_SKILL3297
#define __CPPGEN_GNET_SKILL3297
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3297:public Skill
    {
      public:
        enum
        { SKILL_ID = 3297 };
          Skill3297 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3297Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3297Stub ():SkillStub (3297)
        {
            occupation = 128;
            name = L"¿ñÁé·´ÊÉ";
            nativename = "¿ñÁé·´ÊÉ";
            icon = "¿ñÁé·´ÊÉ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "¹íÍõ×Ú_¿ñÁé·´ÊÉ";
            action[1] = "¹íÍõ×Ú_¿ñÁé·´ÊÉ";
            action[2] = "¹íÍõ×Ú_¿ñÁé·´ÊÉ";
            action[3] = "¹íÍõ×Ú_¿ñÁé·´ÊÉ";
            action[4] = "¹íÍõ×Ú_¿ñÁé·´ÊÉ";
            action[5] = "¹íÍõ×Ú_¿ñÁé·´ÊÉ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¿ñÁé·´ÊÉ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¿ñÁé·´ÊÉ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿ñÁé·´ÊÉ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¿ñÁé·´ÊÉ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¿ñÁé·´ÊÉ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¿ñÁé·´ÊÉ";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3297Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * 3;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 20, 10 * 5, 10);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (20000 + 100 + 2000 * 3);
            skill->GetVictim ()->SetRatio (0.1 * 5);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (100);
            skill->GetVictim ()->SetRetort (1);
            skill->GetVictim ()->SetTime (10000 + 100 + 3000 * 3);
            skill->GetVictim ()->SetImmunesleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (3 > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (60000 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.00001 + 0.1 * 3);
            skill->GetVictim ()->SetValue (0.06 * 3 * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetAddhp2 (1);
            skill->GetVictim ()->SetTime ((10000 + 100 + 3000 * 3) * norm (3));
            skill->GetVictim ()->SetImmunedizzy (1);
            return true;
        }
#endif
    };
}
#endif
