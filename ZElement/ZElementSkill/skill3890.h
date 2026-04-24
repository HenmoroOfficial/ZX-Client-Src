#ifndef __CPPGEN_GNET_SKILL3890
#define __CPPGEN_GNET_SKILL3890
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3890:public Skill
    {
      public:
        enum
        { SKILL_ID = 3890 };
          Skill3890 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3890Stub:public SkillStub
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
      Skill3890Stub ():SkillStub (3890)
        {
            occupation = 129;
            name = L"×£¸£";
            nativename = "×£¸£";
            icon = "Àñ»¨.dds";
            maxlevel = 1;
            maxlearn = 0;
            type = 3;
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
            action[0] = "ÐÝÏÐ";
            action[1] = "ÐÝÏÐ";
            action[2] = "ÐÝÏÐ";
            action[3] = "ÐÝÏÐ";
            action[4] = "ÐÝÏÐ";
            action[5] = "ÐÝÏÐ";
            action[6] = "ÐÝÏÐ";
            action[7] = "ÐÝÏÐ";
            action[8] = "ÐÝÏÐ";
            action[9] = "ÐÝÏÐ";
            action[10] = "ÐÝÏÐ";
            action[11] = "ÐÝÏÐ";
            action[12] = "ÐÝÏÐ";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐÝÏÐ";
            action[16] = "ÐÝÏÐ";
            action[17] = "ÐÝÏÐ";
            action[18] = "ÐÝÏÐ";
            action[19] = "ÐÝÏÐ";
            action[20] = "ÐÝÏÐ";
            action[21] = "ÐÝÏÐ";
            action[22] = "ÐÝÏÐ";
            action[23] = "ÐÝÏÐ";
            action[24] = "ÐÝÏÐ";
            action[25] = "ÐÝÏÐ";
            action[26] = "ÐÝÏÐ";
            action[27] = "ÐÝÏÐ";
            action[28] = "";
            action[29] = "";
            action[30] = "ÐÝÏÐ";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 30;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3890Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 0;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetTmplid () == 52292
                                                   || skill->GetPlayer ()->GetTmplid () == 52293) ? 120 : 0));
            skill->GetVictim ()->SetAmount (180014);
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetUpdatecommondata (1);
            return true;
        }
#endif
    };
}
#endif
